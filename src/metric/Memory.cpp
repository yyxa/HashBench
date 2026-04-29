// src/metric/Memory.cpp
#include "metric/Memory.hpp"

std::string Memory::name() const {
    return "memory_bytes";
}

double Memory::evaluate(const BenchmarkContext& ctx) const {
    return (ctx.hashBits * ctx.hashes.size()) / 8.0; // bytes, not raw bits
}