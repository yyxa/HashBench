// src/metric/Attack.cpp
#include "metric/Attack.hpp"

std::string Attack::name() const {
    return "attack_score";
}

double Attack::evaluate(const BenchmarkContext& ctx) const {
    // heuristic, but consistent scaling
    if (ctx.hashBits >= 256) return 1.0;
    if (ctx.hashBits >= 128) return 0.7;
    if (ctx.hashBits >= 64)  return 0.4;
    return 0.1;
}