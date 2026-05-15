#pragma once
#include "core/IMetric.hpp"
#include <vector>
#include <unordered_map>

class PearsonChi2 : public IMetric {
public:
    std::string name() const override;
    double evaluate(const BenchmarkContext& ctx) const override;
    bool isLowerBetter() const override { return true; }
};