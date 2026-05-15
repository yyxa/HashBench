#include "Speed.hpp"

std::string Speed::name() const {
    return "ns_per_op";
}

double Speed::evaluate(const BenchmarkContext& ctx) const {
    if (ctx.hashes.empty()) return 0.0;
    return static_cast<double>(ctx.duration.count()) / ctx.hashes.size();
}