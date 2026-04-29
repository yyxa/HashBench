// include/core/IDataset.hpp
#pragma once

#include <vector>
#include <string>

class IDataset {
public:
    virtual ~IDataset() = default;

    virtual std::string name() const = 0;
    virtual std::vector<std::string> load() const = 0;
};