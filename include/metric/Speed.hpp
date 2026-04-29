// include/metric/Speed.hpp
#pragma once
#include "core/IMetric.hpp"

class Speed : public IMetric {
public:
    std::string name() const override;
    double evaluate(const BenchmarkContext& ctx) const override;
};