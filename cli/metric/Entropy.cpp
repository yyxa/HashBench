#include "Entropy.hpp"
#include <unordered_map>
#include <cmath>

std::string Entropy::name() const {
    return "entropy";
}

double Entropy::evaluate(const BenchmarkContext& ctx) const {
    if (ctx.hashes.empty() || ctx.tableSize == 0) return 0.0;

    std::unordered_map<size_t, int> freq;
    for (auto h : ctx.hashes)
        freq[h % ctx.tableSize]++;

    double n = static_cast<double>(ctx.hashes.size());
    double entropy = 0.0;
    for (auto& [_, v] : freq) {
        double p = v / n;
        entropy -= p * std::log2(p);
    }
    double maxE = std::log2(ctx.tableSize);
    return maxE > 0 ? entropy / maxE : 0.0;
}