// include/hash/StdHash.hpp
#pragma once
#include "core/IHash.hpp"
#include <functional>

class StdHash : public IHash {
public:
    std::string name() const override {
        return "std::hash";
    }

    uint64_t hash(const std::string& key) const override {
        return std::hash<std::string>{}(key);
    }
};