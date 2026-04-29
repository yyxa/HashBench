// src/metric/Speed.cpp
#include "metric/Speed.hpp"

std::string Speed::name() const {
    return "ns_per_op";
}

double Speed::evaluate(const BenchmarkContext& ctx) const {
    return (double)ctx.duration.count() / ctx.hashes.size();
}