// include/hash/CRC32.hpp
#pragma once

#include "core/IHash.hpp"
#include <array>
#include <cstdint>
#include <string>

class CRC32 : public IHash {
private:
    std::array<uint32_t, 256> table;

    void initTable() {
        const uint32_t poly = 0xEDB88320;

        for (uint32_t i = 0; i < 256; i++) {
            uint32_t crc = i;

            for (int j = 0; j < 8; j++) {
                if (crc & 1)
                    crc = (crc >> 1) ^ poly;
                else
                    crc >>= 1;
            }

            table[i] = crc;
        }
    }

public:
    CRC32() {
        initTable();
    }

    std::string name() const override {
        return "CRC32";
    }

    uint64_t hash(const std::string& key) const override {
        uint32_t crc = 0xFFFFFFFF;

        for (unsigned char c : key) {
            crc = (crc >> 8) ^ table[(crc ^ c) & 0xFF];
        }

        return static_cast<uint64_t>(crc ^ 0xFFFFFFFF);
    }
};