// include/hash/MurmurHash3.hpp
#pragma once

#include "core/IHash.hpp"
#include <cstdint>
#include <string>
#include <cstring>

class MurmurHash3 : public IHash {
private:
    static uint32_t rotl32(uint32_t x, int r) {
        return (x << r) | (x >> (32 - r));
    }

    static uint32_t fmix32(uint32_t h) {
        h ^= h >> 16;
        h *= 0x85ebca6b;
        h ^= h >> 13;
        h *= 0xc2b2ae35;
        h ^= h >> 16;
        return h;
    }

public:
    std::string name() const override {
        return "MurmurHash3_x86_32";
    }

    uint64_t hash(const std::string& key) const override {
        const uint8_t* data = (const uint8_t*)key.data();
        const int len = (int)key.size();

        const uint32_t c1 = 0xcc9e2d51;
        const uint32_t c2 = 0x1b873593;

        uint32_t h1 = 0x12345678;

        const int nblocks = len / 4;

        for (int i = 0; i < nblocks; i++) {
            uint32_t k1;
            std::memcpy(&k1, data + i * 4, sizeof(uint32_t)); // FIX

            k1 *= c1;
            k1 = rotl32(k1, 15);
            k1 *= c2;

            h1 ^= k1;
            h1 = rotl32(h1, 13);
            h1 = h1 * 5 + 0xe6546b64;
        }

        const uint8_t* tail = data + nblocks * 4;
        uint32_t k1 = 0;

        switch (len & 3) {
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1:
                k1 ^= tail[0];
                k1 *= c1;
                k1 = rotl32(k1, 15);
                k1 *= c2;
                h1 ^= k1;
        }

        h1 ^= len;
        h1 = fmix32(h1);

        return h1;
    }
};