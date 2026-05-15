#pragma once
#include "core/IMetric.hpp"

class Entropy : public IMetric {
public:
    std::string name() const override;
    double evaluate(const BenchmarkContext& ctx) const override;
    bool isLowerBetter() const override { return false; }
};