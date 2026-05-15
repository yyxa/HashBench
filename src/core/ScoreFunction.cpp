#include "core/ScoreFunction.hpp"
#include "core/Registry.hpp"
#include "core/BenchmarkContext.hpp"
#include "core/IHash.hpp"
#include "core/IMetric.hpp"
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <chrono>

ScoreFunction::ScoreFunction(ScoreConfig c) : cfg(std::move(c)) {}

std::vector<BenchmarkResult> ScoreFunction::runForDataset(const std::vector<std::string>& data, double loadFactor) {
    auto& reg = Registry::instance();

    std::vector<std::string> activeMetrics;
    for (const auto& [name, w] : cfg.weights) {
        activeMetrics.push_back(name);
    }

    auto hashes = reg.createHashes(cfg.hashes);
    auto metrics = reg.createMetrics(activeMetrics);

    std::vector<BenchmarkResult> results;
    size_t tableSize = std::max<size_t>(100, data.size() / loadFactor);

    for (const auto& h : hashes) {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<uint64_t> hashed;
        hashed.reserve(data.size());
        for (const auto& k : data) hashed.push_back(h->hash(k));
        auto end = std::chrono::high_resolution_clock::now();

        std::unordered_map<size_t, size_t> buckets;
        size_t collisions = 0;
        for (auto x : hashed) {
            if (++buckets[x % tableSize] > 1) collisions++;
        }

        BenchmarkContext ctx;
        ctx.keys = data;
        ctx.hashes = std::move(hashed);
        ctx.tableSize = tableSize;
        ctx.collisions = collisions;
        ctx.loadFactor = loadFactor;
        ctx.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        BenchmarkResult res;
        res.hashName = h->name();

        for (const auto& m : metrics) {
            res.rawMetrics[m->name()] = m->evaluate(ctx);
        }

        results.push_back(res);
    }

    if (results.empty()) return results;

    std::map<std::string, double> minV, maxV;
    for (const auto& m : activeMetrics) {
        minV[m] = 1e18;
        maxV[m] = -1e18;
    }

    for (const auto& r : results) {
        for (const auto& m : activeMetrics) {
            double v = r.rawMetrics.at(m);
            minV[m] = std::min(minV[m], v);
            maxV[m] = std::max(maxV[m], v);
        }
    }

    for (auto& r : results) {
        double sumScore = 0.0;
        double geomScore = 1.0;
        const double eps = 1e-8;

        for (size_t i = 0; i < metrics.size(); ++i) {
            const std::string& mname = activeMetrics[i];
            double raw = r.rawMetrics[mname];
            double range = maxV[mname] - minV[mname];
            double norm = (range > 1e-9) ? (raw - minV[mname]) / range : 0.5;

            if (metrics[i]->isLowerBetter()) {
                norm = 1.0 - norm;
            }

            r.normMetrics[mname] = norm;

            double weight = cfg.weights[mname];
            sumScore += weight * norm;
            geomScore *= std::pow(norm + eps, weight);
        }

        r.sumScore = sumScore;
        r.geomScore = geomScore;

        double lambda = cfg.lambdas.empty() ? 0.5 : cfg.lambdas[0];
        r.finalScore = lambda * sumScore + (1.0 - lambda) * geomScore;
    }

    return results;
}