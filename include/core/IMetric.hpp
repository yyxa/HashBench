#pragma once
#include <string>
#include "BenchmarkContext.hpp"

class IMetric {
public:
    virtual ~IMetric() = default;
    virtual std::string name() const = 0;
    virtual double evaluate(const BenchmarkContext& ctx) const = 0;
    virtual bool isLowerBetter() const = 0;
};