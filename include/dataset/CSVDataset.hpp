// include/dataset/CSVDataset.hpp
#pragma once

#include "core/IDataset.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

class CSVDataset : public IDataset {
    std::string path;
    size_t keyIndex;

public:
    CSVDataset(std::string p, size_t keyIdx = 0)
        : path(std::move(p)), keyIndex(keyIdx) {}

    std::string name() const override {
        return "csv_dataset: " + path + " (key col " + std::to_string(keyIndex) + ")";
    }

    std::vector<std::string> load() const override {
        std::vector<std::string> result;

        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("Cannot open dataset: " + path);

        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string cell;

            size_t col = 0;
            std::string key;

            while (std::getline(ss, cell, ',')) {
                if (col == keyIndex)
                    key = cell;
                col++;
            }

            if (!key.empty())
                result.push_back(key);
        }

        return result;
    }
};