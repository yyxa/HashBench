// src/metric/Throughput.cpp
#include "metric/Throughput.hpp"

std::string Throughput::name() const {
    return "ops_per_sec";
}

double Throughput::evaluate(const BenchmarkContext& ctx) const {
    double sec = ctx.duration.count() / 1e9;
    return ctx.hashes.size() / sec;
}