#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>
#include "CoreTypes.hpp"

class Registry {
private:
    std::unordered_map<std::string, std::function<HashPtr()>> hashFactories;
    std::unordered_map<std::string, std::function<MetricPtr()>> metricFactories;

public:
    static Registry& instance();

    void registerHash(const std::string& name, std::function<HashPtr()> f);
    void registerMetric(const std::string& name, std::function<MetricPtr()> f);

    HashList createHashes(const std::vector<std::string>& names) const;
    MetricList createMetrics(const std::vector<std::string>& names) const;

    void validateHashes(const std::vector<std::string>& names) const;
    void validateMetrics(const std::vector<std::string>& names) const;
};