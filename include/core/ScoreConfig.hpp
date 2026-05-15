#pragma once
#include <vector>
#include <string>
#include <map>

struct DatasetConfig {
    std::string path;
    size_t keyIndex = 0;
};

struct ScoreConfig {
    std::vector<double> lambdas;
    std::vector<double> loadFactors;
    std::map<std::string, double> weights;
    std::vector<std::string> hashes;
    std::vector<DatasetConfig> datasets;
};