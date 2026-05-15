#include "CSVDataset.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

CSVDataset::CSVDataset(std::string p, size_t keyIdx)
    : path(std::move(p)), keyIndex(keyIdx) {}

std::string CSVDataset::name() const {
    return "csv:" + path + "(key=" + std::to_string(keyIndex) + ")";
}

std::vector<std::string> CSVDataset::load() const {
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
            if (col == keyIndex) {
                key = cell;
            }
            col++;
        }
        if (!key.empty())
            result.push_back(key);
    }
    return result;
}