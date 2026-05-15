#pragma once
#include "core/IMetric.hpp"

class CollisionRate : public IMetric {
public:
    std::string name() const override;
    double evaluate(const BenchmarkContext& ctx) const override;
    bool isLowerBetter() const override { return true; }
};