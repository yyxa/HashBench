#include "core/Registry.hpp"
#include <stdexcept>

Registry& Registry::instance() {
    static Registry r;
    return r;
}

void Registry::registerHash(const std::string& name, std::function<HashPtr()> f) {
    hashFactories[name] = std::move(f);
}

void Registry::registerMetric(const std::string& name, std::function<MetricPtr()> f) {
    metricFactories[name] = std::move(f);
}

HashList Registry::createHashes(const std::vector<std::string>& names) const {
    HashList out;
    for (const auto& n : names) {
        auto it = hashFactories.find(n);
        if (it == hashFactories.end()) throw std::runtime_error("Unknown hash: " + n);
        out.push_back(it->second());
    }
    return out;
}

MetricList Registry::createMetrics(const std::vector<std::string>& names) const {
    MetricList out;
    for (const auto& n : names) {
        auto it = metricFactories.find(n);
        if (it == metricFactories.end()) throw std::runtime_error("Unknown metric: " + n);
        out.push_back(it->second());
    }
    return out;
}

void Registry::validateHashes(const std::vector<std::string>& names) const {
    for (const auto& h : names)
        if (hashFactories.find(h) == hashFactories.end())
            throw std::runtime_error("Unknown hash: " + h);
}

void Registry::validateMetrics(const std::vector<std::string>& names) const {
    for (const auto& m : names)
        if (metricFactories.find(m) == metricFactories.end())
            throw std::runtime_error("Unknown metric: " + m);
}