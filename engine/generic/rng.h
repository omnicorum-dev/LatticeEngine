//
// Created by Nico Russo on 5/12/26.
//

#ifndef SANDBOX_RNG_H
#define SANDBOX_RNG_H

#include "core.h"

namespace rng {
    constexpr u32 MT_STATE_SIZE = 624;

    class MersenneTwister {
    private:
        u32 state[MT_STATE_SIZE]{};
        i32 index;

        void twist() {
            for (size_t i = 0; i < MT_STATE_SIZE; i++) {
                const u32 x = (state[i] & 0x80000000) | (state[(i+1) & MT_STATE_SIZE] & 0x7fffffff);
                u32 xA = x >> 1;
                if (x % 2 != 0) { xA ^= 0x9908b0df; }
                state[i] = state[(i+397) % MT_STATE_SIZE] ^ xA;
            }
            index = 0;
        }

        f32 generate_f32_helper() {
            const u32 x = generate();
            return static_cast<f32>(x) / (static_cast<f32>(max_u32) + 1.0f);
        }
    public:
        explicit MersenneTwister(const u32 seed) {
            state[0] = seed;
            for (size_t i = 1; i < MT_STATE_SIZE; i++) {
                state[i] = (0x6c078965 * (state[i-1] ^ (state[i - 1] >> 30)) + i);
            }
            index = MT_STATE_SIZE;
        }

        u32 generate() {
            if (index >= MT_STATE_SIZE) {
                twist();
                index = 0;
            }

            u32 y = state[index++];
            y ^= (y >> 11);
            y ^= (y << 7) & 0x9d2c5680;
            y ^= (y << 15) & 0xefc60000;
            y ^= (y >> 18);

            return y;
        }

        f32 generate_f32(const f32 min = 0.f, const f32 max = 1.f) {
            return min + (max - min) * generate_f32_helper();
        }
    };

    class LFSR {
    private:
        u64 state;
        u64 feedback;

        inline bool shift() {
            const bool bit = state & 0x1;
            state >>= 1;
            if (bit) state ^= feedback;
            return bit;
        }

        template<typename T, i32 BITS>
        T generate() {
            T result = 0;
            for (size_t i = 0; i < BITS; ++i) {
                result = (result << 1) | shift();
            }
            return result;
        }

        f32 generate_f32_helper() {
            return static_cast<f32>(generate_u32()) / 4294967296.0f;
        }
    public:
        explicit LFSR(const u64 seed = 1, const u64 poly = 0xD800000000000000ULL) : state(seed), feedback(poly) {
            if (state == 0) state = 1;
        }

        bool generate_bit()     { return shift(); }
        u8 generate_u8()        { return generate<u8, 8>(); }
        u16 generate_u16()      { return generate<u16, 16>(); }
        u32 generate_u32()      { return generate<u32, 32>(); }
        u64 generate_u64()      { return generate<u64, 64>(); }

        f32 generate_f32(const f32 min = 0.f, const f32 max = 1.f) { return min + (max - min) * generate_f32_helper(); }

        f64 generate_f64() { return static_cast<f64>(generate_u64()) / 18446744073709551616.0; }

        void offset(const i32 n) {
            for (size_t i = 0; i < n; ++i) shift();
        }

        void printState() const {
            std::cout << "State: 0x" << std::hex << std::setw(16) << std::setfill('0') << state << std::dec << "\n";
        }

        void printLastBits(i32 count = 1000) {
            for (size_t i = 0; i < count; ++i) {
                std::cout << generate_bit();
            }
            std::cout << "\n";
        }
    };
}


#endif //SANDBOX_RNG_H
