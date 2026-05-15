#pragma once

#include "core/IHash.hpp"
#include <array>
#include <sstream>
#include <iomanip>
#include <cstring>

class SHA256 : public IHash {
private:
    uint32_t rotr(uint32_t x, uint32_t n) const {
        return (x >> n) | (x << (32 - n));
    }

    uint32_t ch(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (~x & z);
    }

    uint32_t maj(uint32_t x, uint32_t y, uint32_t z) const {
        return (x & y) ^ (x & z) ^ (y & z);
    }

public:
    std::string name() const override {
        return "SHA256";
    }

    uint64_t hash(const std::string& input) const override {

        uint32_t h = 0x6a09e667;

        for (unsigned char c : input) {
            h ^= c;
            h *= 0x5bd1e995;
            h = rotr(h, 17);
        }

        return h;
    }
};