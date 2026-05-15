#pragma once
#include "core/IDataset.hpp"
#include <string>

class CSVDataset : public IDataset {
    std::string path;
    size_t keyIndex;
public:
    CSVDataset(std::string p, size_t keyIdx = 0);
    std::string name() const override;
    std::vector<std::string> load() const override;
};