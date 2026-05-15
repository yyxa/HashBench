#include "core/ConfigLoader.hpp"
#include <yaml-cpp/yaml.h>

ScoreConfig loadConfig(const std::string& path) {
    YAML::Node node = YAML::LoadFile(path);
    ScoreConfig cfg;

    if (auto w = node["weights"]) {
        for (auto it = w.begin(); it != w.end(); ++it) {
            std::string name = it->first.as<std::string>();
            double val = it->second.as<double>(0.0);
            if (val > 0.0)
                cfg.weights[name] = val;
        }
    }

    cfg.hashes = node["hashes"].as<std::vector<std::string>>();

    if (node["lambdas"]) cfg.lambdas = node["lambdas"].as<std::vector<double>>();
    if (node["load_factors"]) cfg.loadFactors = node["load_factors"].as<std::vector<double>>();

    if (node["datasets"]) {
        for (auto d : node["datasets"]) {
            DatasetConfig dc;
            dc.path = d["path"].as<std::string>();
            dc.keyIndex = d["key"].as<size_t>(0);
            cfg.datasets.push_back(dc);
        }
    }

    if (cfg.lambdas.empty()) cfg.lambdas = {0.5};
    if (cfg.loadFactors.empty()) cfg.loadFactors = {0.5};

    return cfg;
}