// src/metric/Entropy.cpp
#include "metric/Entropy.hpp"
#include <unordered_map>
#include <cmath>

std::string Entropy::name() const {
    return "entropy";
}

double Entropy::evaluate(const BenchmarkContext& ctx) const {
    std::unordered_map<size_t, int> freq;

    for (auto h : ctx.hashes)
        freq[h % ctx.tableSize]++;

    double n = (double)ctx.hashes.size();
    double entropy = 0.0;

    for (auto& [_, v] : freq) {
        double p = v / n;
        entropy -= p * std::log2(p);
    }

    // normalization to [0..1]
    double maxEntropy = std::log2(ctx.tableSize);
    if (maxEntropy == 0) return 0.0;

    return entropy / maxEntropy;
}