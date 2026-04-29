// include/metric/ProbeLength.hpp
#pragma once
#include "core/IMetric.hpp"

class ProbeLength : public IMetric {
public:
    std::string name() const override {
        return "avg_probe";
    }

    double evaluate(const BenchmarkContext& ctx) const override {
        return (double)ctx.totalProbes / ctx.hashes.size();
    }
};