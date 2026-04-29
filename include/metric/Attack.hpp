// include/metric/Attack.hpp
#pragma once
#include "core/IMetric.hpp"

class Attack : public IMetric {
public:
    std::string name() const override;
    double evaluate(const BenchmarkContext& ctx) const override;
};