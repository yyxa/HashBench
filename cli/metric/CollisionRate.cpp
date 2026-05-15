#include "CollisionRate.hpp"

std::string CollisionRate::name() const {
    return "collision";
}

double CollisionRate::evaluate(const BenchmarkContext& ctx) const {
    if (ctx.hashes.empty()) return 0.0;
    return static_cast<double>(ctx.collisions) / ctx.hashes.size();
}