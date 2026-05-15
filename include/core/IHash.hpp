#pragma once

#include <string>
#include <cstdint>

class IHash {
public:
    virtual ~IHash() = default;

    virtual std::string name() const = 0;
    virtual uint64_t hash(const std::string& key) const = 0;
};