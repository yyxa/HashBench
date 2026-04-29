// src/core/ScoreFunction.cpp
#include "core/ScoreFunction.hpp"
#include "core/Registry.hpp"
#include "core/BenchmarkContext.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace fs = std::filesystem;

#ifndef PROJECT_ROOT
#define PROJECT_ROOT "."
#endif

struct Row {
    std::string hashName;
    double score = 0.0;
    std::unordered_map<std::string, double> metrics;
};

static bool isLowerBetter(const std::string& m) {
    return m == "collision_rate" ||
           m == "ns_per_op" ||
           m == "memory_bits";
}

// -----------------------------
// FAST hash-table simulation (O(n))
// -----------------------------
static void simulateHashing(
    const std::vector<uint64_t>& hashes,
    size_t tableSize,
    size_t& collisions,
    size_t& probesTotal)
{
    std::unordered_map<size_t, size_t> buckets;
    buckets.reserve(hashes.size());

    collisions = 0;
    probesTotal = 0;

    for (auto h : hashes) {
        size_t idx = h % tableSize;

        size_t count = ++buckets[idx];

        if (count > 1)
            collisions += (count - 1);

        probesTotal += count;
    }
}

void ScoreFunction::run(size_t /*unused*/) {

    auto& reg = Registry::instance();

    auto hashes = reg.createHashes();
    auto metrics = reg.createMetrics();
    auto datasets = reg.createDatasets();

    std::vector<double> loadFactors = {0.25, 0.5, 0.75, 1.0, 2.0};

    fs::path outputDir = fs::path(PROJECT_ROOT) / "output";
    fs::create_directories(outputDir);

    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);

    std::ofstream out(outputDir / ("score_" + std::to_string(t) + ".txt"));

    std::unordered_map<std::string, double> globalScores;

    for (auto& ds : datasets) {

        auto data = ds->load();

        std::cout << "\n==== DATASET: " << ds->name() << " ====\n";

        for (double lf : loadFactors) {

            size_t tableSize = std::max<size_t>(1, (size_t)(data.size() / lf));

            std::cout << "\n-- Load Factor: " << lf
                      << " (tableSize=" << tableSize << ") --\n";

            std::vector<Row> rows;

            // ========================
            // compute metrics
            // ========================
            for (auto& hash : hashes) {

                std::vector<uint64_t> hashed;
                hashed.reserve(data.size());

                auto start = std::chrono::high_resolution_clock::now();

                for (auto& k : data)
                    hashed.push_back(hash->hash(k));

                auto end = std::chrono::high_resolution_clock::now();

                size_t collisions = 0;
                size_t probes = 0;

                simulateHashing(hashed, tableSize, collisions, probes);

                BenchmarkContext ctx;
                ctx.keys = data;
                ctx.hashes = hashed;
                ctx.tableSize = tableSize;
                ctx.collisions = collisions;
                ctx.totalProbes = probes;
                ctx.loadFactor = (double)data.size() / tableSize;
                ctx.duration =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

                ctx.hashBits =
                    (hash->name().find("SHA256") != std::string::npos) ? 256 :
                    (hash->name().find("64") != std::string::npos) ? 64 : 32;

                Row r;
                r.hashName = hash->name();

                for (auto& m : metrics)
                    r.metrics[m->name()] = m->evaluate(ctx);

                rows.push_back(r);
            }

            // ========================
            // normalize
            // ========================
            std::unordered_map<std::string, double> minV, maxV;

            for (auto& m : metrics) {
                minV[m->name()] = 1e18;
                maxV[m->name()] = -1e18;
            }

            for (auto& r : rows) {
                for (auto& [k, v] : r.metrics) {
                    minV[k] = std::min(minV[k], v);
                    maxV[k] = std::max(maxV[k], v);
                }
            }

            // ========================
            // scoring
            // ========================
            for (auto& r : rows) {

                double score = 0.0;

                for (auto& [k, v] : r.metrics) {

                    double norm = 0.0;

                    if (maxV[k] != minV[k])
                        norm = (v - minV[k]) / (maxV[k] - minV[k]);

                    if (isLowerBetter(k))
                        norm = 1.0 - norm;

                    score += norm;
                }

                r.score = score;
                globalScores[r.hashName] += score;
            }

            // ========================
            // sort
            // ========================
            std::sort(rows.begin(), rows.end(),
                [](auto& a, auto& b) {
                    return a.score > b.score;
                });

            // ========================
            // output
            // ========================
            std::cout << std::left << std::setw(25) << "HASH";

            for (auto& m : metrics)
                std::cout << std::setw(15) << m->name();

            std::cout << std::setw(15) << "SCORE\n";

            for (auto& r : rows) {

                std::cout << std::setw(25) << r.hashName;

                for (auto& m : metrics)
                    std::cout << std::setw(15) << r.metrics[m->name()];

                std::cout << std::setw(15) << r.score << "\n";
            }
        }
    }

    // ========================
    // FINAL SCORE
    // ========================
    std::cout << "\n==== FINAL SCORE ====\n";

    std::vector<std::pair<std::string, double>> finalVec(
        globalScores.begin(), globalScores.end()
    );

    std::sort(finalVec.begin(), finalVec.end(),
        [](auto& a, auto& b) {
            return a.second > b.second;
        });

    for (auto& [name, score] : finalVec) {
        std::cout << std::setw(25) << name
                  << std::setw(15) << score << "\n";
    }

    std::cout << "\nSaved to output dir\n";
}