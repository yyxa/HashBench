// include/core/Registry.hpp
#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "IHash.hpp"
#include "IMetric.hpp"
#include "IDataset.hpp"

class Registry {
public:
    static Registry& instance();

    using HashFactory = std::function<std::shared_ptr<IHash>()>;
    using MetricFactory = std::function<std::shared_ptr<IMetric>()>;
    using DatasetFactory = std::function<std::shared_ptr<IDataset>()>;

    std::vector<HashFactory> hashFactories;
    std::vector<MetricFactory> metricFactories;
    std::vector<DatasetFactory> datasetFactories;

    void registerHash(HashFactory f);
    void registerMetric(MetricFactory f);
    void registerDataset(DatasetFactory f);

    std::vector<std::shared_ptr<IHash>> createHashes() const;
    std::vector<std::shared_ptr<IMetric>> createMetrics() const;
    std::vector<std::shared_ptr<IDataset>> createDatasets() const;
};