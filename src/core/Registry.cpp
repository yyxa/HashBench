// src/core/Registry.cpp
#include "core/Registry.hpp"

Registry& Registry::instance() {
    static Registry r;
    return r;
}

void Registry::registerHash(HashFactory f) {
    hashFactories.push_back(f);
}

void Registry::registerMetric(MetricFactory f) {
    metricFactories.push_back(f);
}

void Registry::registerDataset(DatasetFactory f) {
    datasetFactories.push_back(f);
}

std::vector<std::shared_ptr<IHash>> Registry::createHashes() const {
    std::vector<std::shared_ptr<IHash>> result;
    for (auto& f : hashFactories)
        result.push_back(f());
    return result;
}

std::vector<std::shared_ptr<IMetric>> Registry::createMetrics() const {
    std::vector<std::shared_ptr<IMetric>> result;
    for (auto& f : metricFactories)
        result.push_back(f());
    return result;
}

std::vector<std::shared_ptr<IDataset>> Registry::createDatasets() const {
    std::vector<std::shared_ptr<IDataset>> result;
    for (auto& f : datasetFactories)
        result.push_back(f());
    return result;
}