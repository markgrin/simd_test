#include <ctime>
#include <iostream>
#include <numeric>
#include <vector>

#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

namespace {

constexpr int bigint = 555555555;

void func_naive_latency(int spin) {
    volatile int j = 3;
    for (int i = 0; i < spin; i++) {
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
        j = j * bigint;
    }
}

void func_naive_throughoutput(int spin) {
    volatile int k = 3;
    volatile int j = bigint;
    for (int i = 0; i < spin; i++) {
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
        k = j * bigint;
    }
}

void func_2_latency(int spin) {
    spin /= 2;
    volatile auto j = _mm_set_epi32(bigint, bigint, bigint, bigint);
    volatile auto k = _mm_set_epi32(bigint, bigint, bigint, bigint);
    for (int i = 0; i < spin; i++) {
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
        j = _mm_mul_epi32(j, k);
    }
}

void func_2_throughoutput(int spin) {
    spin /= 2;
    volatile auto j = _mm_set_epi32(bigint, bigint, bigint, bigint);
    volatile auto k = _mm_set_epi32(bigint, bigint, bigint, bigint);
    volatile auto t = _mm_set_epi32(bigint, bigint, bigint, bigint);
    for (int i = 0; i < spin; i++) {
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
        t = _mm_mul_epi32(j, k);
    }
}

void func_4_latency(int spin) {
    spin /= 4;
    volatile auto j = _mm256_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    volatile auto k = _mm256_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    for (int i = 0; i < spin; i++) {
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
        j = _mm256_mul_epi32(j, k);
    }
}

void func_4_throughoutput(int spin) {
    spin /= 4;
    volatile auto j = _mm256_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    volatile auto k = _mm256_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    volatile auto t = k;
    for (int i = 0; i < spin; i++) {
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
        t = _mm256_mul_epi32(j, k);
    }
}

void func_8_latency(int spin) {
    spin /= 8;
    volatile auto j = _mm512_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    volatile auto k = _mm512_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    for (int i = 0; i < spin; i++) {
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
        j = _mm512_mul_epi32(j, k);
    }
}

void func_8_throughoutput(int spin) {
    spin /= 8;
    volatile auto j = _mm512_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    volatile auto k = _mm512_set_epi32(bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint, bigint);
    volatile auto t = k;
    for (int i = 0; i < spin; i++) {
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
        k = _mm512_mul_epi32(j, k);
    }
}

}

int main () {
    using func_ptr_t = void (*) (int);
    struct callinfo_t {
        std::string name;
        func_ptr_t ptr;
    };
    std::vector<callinfo_t> callinfos;
    callinfos.push_back({"func_naive_latency", func_naive_latency});
    callinfos.push_back({"func_naive_throughoutput", func_naive_throughoutput});
    callinfos.push_back({"func_2_latency", func_2_latency});
    callinfos.push_back({"func_2_throughoutput", func_2_throughoutput});
    callinfos.push_back({"func_4_latency", func_4_latency});
    callinfos.push_back({"func_4_throughoutput", func_4_throughoutput});
    callinfos.push_back({"func_8_latency", func_8_latency});
    callinfos.push_back({"func_8_throughoutput", func_8_throughoutput});
    for (auto&& callinfo : callinfos) {
        constexpr std::size_t repeat = 10;
        double elapseds[repeat];
        for (auto&& elapsed : elapseds) {
            int spin = 10000000;
            std::clock_t start_timestamp = std::clock();
            callinfo.ptr(spin);
            std::clock_t stop_timestamp = std::clock();
            elapsed = 1000.0 * (stop_timestamp - start_timestamp) / CLOCKS_PER_SEC;
        }
        double avg = std::accumulate(elapseds, elapseds + repeat, 0.0) / repeat;
        std::cout << "{\"name\":\"" << callinfo.name << "\", \"avg\":" << avg << "}\n";
    }
 
}

