#pragma once

#include "core/IHash.hpp"
#include <cstdint>
#include <string>
#include <cstring>

class CityHash : public IHash {
private:
    static uint64_t rotate(uint64_t val, int shift) {
        return (val >> shift) | (val << (64 - shift));
    }

    static uint64_t shiftMix(uint64_t val) {
        return val ^ (val >> 47);
    }

public:
    std::string name() const override {
        return "CityHash64";
    }

    uint64_t hash(const std::string& s) const override {
        const char* data = s.data();
        size_t len = s.size();

        if (len <= 16) {
            uint64_t a = 0, b = 0;
            std::memcpy(&a, data, len > 8 ? 8 : len);
            std::memcpy(&b, data + len - (len > 8 ? 8 : len), len > 8 ? 8 : len);

            uint64_t mul = 0x9ddfea08eb382d69ULL;
            uint64_t c = (len + (a << 3));

            return (c ^ b) * mul;
        }

        uint64_t h = len;

        size_t i = 0;
        while (i + 8 <= len) {
            uint64_t k;
            std::memcpy(&k, data + i, 8);

            h ^= k;
            h = rotate(h, 27);
            h *= 5;
            h += 0x52dce729;

            i += 8;
        }

        return shiftMix(h);
    }
};