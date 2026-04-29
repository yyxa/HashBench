// include/metric/LoadFactor.hpp
#pragma once
#include "core/IMetric.hpp"

class LoadFactor : public IMetric {
public:
    std::string name() const override {
        return "load_factor";
    }

    double evaluate(const BenchmarkContext& ctx) const override {
        return ctx.loadFactor;
    }
};