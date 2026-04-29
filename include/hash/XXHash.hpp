// include/hash/XXHash.hpp
#pragma once

#include "core/IHash.hpp"
#include <cstdint>
#include <string>
#include <cstring>

class XXHash : public IHash {
private:
    static uint64_t rotl(uint64_t x, int r) {
        return (x << r) | (x >> (64 - r));
    }

public:
    std::string name() const override {
        return "xxHash64";
    }

    uint64_t hash(const std::string& key) const override {
        const uint8_t* p = (const uint8_t*)key.data();
        size_t len = key.size();

        const uint64_t prime1 = 11400714785074694791ULL;
        const uint64_t prime2 = 14029467366897019727ULL;
        const uint64_t prime3 = 1609587929392839161ULL;
        const uint64_t prime4 = 9650029242287828579ULL;
        const uint64_t prime5 = 2870177450012600261ULL;

        uint64_t h = prime5 + len;

        size_t i = 0;
        while (i + 8 <= len) {
            uint64_t k;
            std::memcpy(&k, p + i, 8);

            k *= prime2;
            k = rotl(k, 31);
            k *= prime1;

            h ^= k;
            h = rotl(h, 27) * prime1 + prime4;

            i += 8;
        }

        while (i < len) {
            h ^= p[i] * prime5;
            h = rotl(h, 11) * prime1;
            i++;
        }

        h ^= h >> 33;
        h *= prime2;
        h ^= h >> 29;
        h *= prime3;
        h ^= h >> 32;

        return h;
    }
};