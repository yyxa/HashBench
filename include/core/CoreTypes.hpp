#pragma once
#include <memory>
#include <vector>

class IHash;
class IMetric;
class IDataset;

using HashPtr = std::shared_ptr<IHash>;
using MetricPtr = std::shared_ptr<IMetric>;
using DatasetPtr = std::shared_ptr<IDataset>;

using HashList = std::vector<HashPtr>;
using MetricList = std::vector<MetricPtr>;