// include/core/BenchmarkContext.hpp
#pragma once

#include <vector>
#include <string>
#include <chrono>

struct BenchmarkContext {
    std::vector<std::string> keys;
    std::vector<uint64_t> hashes;

    size_t tableSize = 0;

    // simulation
    size_t collisions = 0;
    size_t totalProbes = 0;
    double loadFactor = 0.0;

    std::chrono::nanoseconds duration;

    int hashBits = 32;
};