#include "PearsonChi2.hpp"
#include <cmath>

std::string PearsonChi2::name() const {
    return "pearson";
}

double PearsonChi2::evaluate(const BenchmarkContext& ctx) const {
    if (ctx.tableSize == 0 || ctx.hashes.empty()) return 0.0;

    std::unordered_map<size_t, size_t> counts;
    for (auto h : ctx.hashes) {
        counts[h % ctx.tableSize]++;
    }

    double n = static_cast<double>(ctx.hashes.size());
    double M = static_cast<double>(ctx.tableSize);
    double expected = n / M;
    double chi2 = 0.0;

    for (auto& [_, observed] : counts) {
        double diff = observed - expected;
        chi2 += (diff * diff) / expected;
    }

    size_t empty = ctx.tableSize - counts.size();
    if (empty > 0) {
        chi2 += empty * (expected);
    }
    return chi2;
}