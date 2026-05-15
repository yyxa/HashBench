#include "core/ConfigLoader.hpp"
#include "core/ScoreFunction.hpp"
#include "core/Registry.hpp"

#include "dataset/CSVDataset.hpp"

#include "hash/CRC32.hpp"
#include "hash/MurmurHash3.hpp"
#include "hash/XXHash.hpp"
#include "hash/CityHash.hpp"
#include "hash/SHA256.hpp"

#include "metric/CollisionRate.hpp"
#include "metric/Speed.hpp"
#include "metric/Entropy.hpp"
#include "metric/PearsonChi2.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <vector>

namespace fs = std::filesystem;

void registerAll() {
    auto& r = Registry::instance();

    r.registerHash("CRC32",       []() { return std::make_shared<CRC32>(); });
    r.registerHash("MurmurHash3", []() { return std::make_shared<MurmurHash3>(); });
    r.registerHash("XXHash",      []() { return std::make_shared<XXHash>(); });
    r.registerHash("CityHash",    []() { return std::make_shared<CityHash>(); });
    r.registerHash("SHA256",      []() { return std::make_shared<SHA256>(); });

    r.registerMetric("collision",  []() { return std::make_shared<CollisionRate>(); });
    r.registerMetric("ns_per_op",  []() { return std::make_shared<Speed>(); });
    r.registerMetric("entropy",    []() { return std::make_shared<Entropy>(); });
    r.registerMetric("pearson",    []() { return std::make_shared<PearsonChi2>(); });
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config.yaml>\n";
        return 1;
    }

    registerAll();

    try {
        auto cfg = loadConfig(argv[1]);
        ScoreFunction scorer(cfg);

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&t);
        std::ostringstream ss;
        ss << "run_" << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
        std::string base = ss.str();

        fs::path runDir = fs::path("output") / base;
        fs::create_directories(runDir);

        fs::path csvPath = runDir / (base + ".csv");
        std::ofstream csv(csvPath);

        std::vector<std::string> metrics;
        for (const auto& p : cfg.weights) {
            metrics.push_back(p.first);
        }

        csv << "lambda,load_factor,dataset,hash_name";
        for (const auto& m : metrics) csv << ",raw_" << m;
        for (const auto& m : metrics) csv << ",norm_" << m;
        csv << ",sum_score,geom_score,final_score\n";

        for (const auto& dsCfg : cfg.datasets) {
            CSVDataset ds(dsCfg.path, dsCfg.keyIndex);
            auto data = ds.load();

            for (double lf : cfg.loadFactors) {
                auto results = scorer.runForDataset(data, lf);

                for (const auto& r : results) {
                    csv << (cfg.lambdas.empty() ? 0.5 : cfg.lambdas[0]) << ","
                        << lf << "," << dsCfg.path << "," << r.hashName;

                    for (const auto& m : metrics) csv << "," << r.rawMetrics.at(m);
                    for (const auto& m : metrics) csv << "," << r.normMetrics.at(m);
                    csv << "," << r.sumScore << "," << r.geomScore << "," << r.finalScore << "\n";
                }
            }
        }

        std::cout << "Results saved to: " << csvPath << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}