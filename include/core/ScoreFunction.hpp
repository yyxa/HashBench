#pragma once
#include "ScoreConfig.hpp"
#include "CoreTypes.hpp"
#include <vector>
#include <string>
#include <map>

struct BenchmarkResult {
    std::string hashName;
    std::map<std::string, double> rawMetrics;
    std::map<std::string, double> normMetrics;
    double sumScore = 0.0;
    double geomScore = 0.0;
    double finalScore = 0.0;
};

class ScoreFunction {
    ScoreConfig cfg;
public:
    explicit ScoreFunction(ScoreConfig c);
    
    std::vector<BenchmarkResult> runForDataset(const std::vector<std::string>& data, double loadFactor);
};