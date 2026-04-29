// include/metric/Throughput.hpp
#pragma once
#include "core/IMetric.hpp"

class Throughput : public IMetric {
public:
    std::string name() const override;
    double evaluate(const BenchmarkContext& ctx) const override;
};