#include "polyline_chunking.hpp"
#include <iostream>
#include <openssl/sha.h>

POLYLINE_Chunking::POLYLINE_Chunking() {
}

POLYLINE_Chunking::POLYLINE_Chunking(const Config& config) {
}

uint64_t POLYLINE_Chunking::find_cutpoint(char* data, uint64_t size) {
    if (size < 2) {
        return size;
    }
    uint16_t square = static_cast<unsigned char>(data[0]) + static_cast<unsigned char>(data[1]);
    int16_t delta_f = 0, delta_prev = data[1] - data[0];
    int16_t double_delta_f = 0;
    uint64_t idx = 2;

    uint64_t large_mask = 0b0000000000000000100100100100100100100100100100100100100100100100;
    uint64_t small_mask = 0b0000000000000000000000000010010010000001001001001000000000100100100;


    while (idx < size) {
        square = (square << 1) + GEAR_TABLE[256 + data[idx]];

        int16_t delta_cur = data[idx] - data[idx - 1];
        delta_f = (delta_f << 1) + GEAR_TABLE[256 + delta_cur];

        double_delta_f = (double_delta_f << 1) + GEAR_TABLE[256 + delta_cur - delta_prev];
        delta_prev = delta_cur;

        // square += data[idx];

        // int16_t delta_cur = data[idx] - data[idx - 1];
        // delta_f += delta_cur;

        // double_delta_f += delta_cur - delta_prev;
        // delta_prev = delta_cur;
        
        // uint64_t hash = (double_delta_f << 32) + (delta_f << 16) + square;
        //std::cout << "start " << square << ' ' << delta_f << ' ' << double_delta_f << ' ' << (((double_delta_f << 32) + (delta_f << 16) + square) & mask) << "\n";

        if (idx < 8192) {
            if (!(((double_delta_f << 32) + (delta_f << 16) + square) & large_mask)) {
                return idx;
            }
        } else {
            if (!(((double_delta_f << 32) + (delta_f << 16) + square) & small_mask)) {
                return idx;
            }
        }

        
        idx += 1;
    }

    return idx;
}
