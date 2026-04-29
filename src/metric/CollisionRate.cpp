// src/metric/CollisionRate.cpp
#include "metric/CollisionRate.hpp"

std::string CollisionRate::name() const {
    return "collision_rate";
}

double CollisionRate::evaluate(const BenchmarkContext& ctx) const {
    return (double)ctx.collisions / ctx.hashes.size();
}