#include <ctime>
#include <iostream>
#include <numeric>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <cstdlib>

#include <xmmintrin.h>
#include <smmintrin.h>
#include <immintrin.h>

namespace {

double func(double x) {
    return x*x;
}

auto create_data (std::size_t size, double step) {
    std::unique_ptr<double[], decltype(std::free) *> data(static_cast<double*>(aligned_alloc(64, size * sizeof(double))), std::free);
    for (std::size_t i = 0; i < size; i++) {
        double point = step * i;
        data[i] = func(point);
    }
    return data;
}

double integrate(double* values, std::size_t size, double step) {
    volatile double sum = 0;
    for (std::size_t i = 0; i < size; i++) {
        sum += values[i] * step;
    }
    return sum;
}

double integrate2(double* values, std::size_t size, double step) {
    auto sum_vector = _mm_set_pd(0, 0);
    auto step_vector = _mm_set_pd(step, step);
    for (std::size_t i = 0; i < size; i += 2) {
        auto value_vector = _mm_load_pd (values + i);
        value_vector = _mm_mul_pd(value_vector, step_vector);
        sum_vector = _mm_add_pd(sum_vector, value_vector);
    }
    alignas(64) double result[2];
    _mm_store_pd(result, sum_vector);
    return result[0] + result[1];
}

double integrate4(double* values, std::size_t size, double step) {
    auto sum_vector = _mm256_set_pd(0, 0, 0, 0);
    auto step_vector = _mm256_set_pd(step, step, step, step);
    for (std::size_t i = 0; i < size; i += 4) {
        auto value_vector = _mm256_load_pd (values + i);
        value_vector = _mm256_mul_pd(value_vector, step_vector);
        sum_vector = _mm256_add_pd(sum_vector, value_vector);
    }
    alignas(64) double result[4];
    _mm256_store_pd(result, sum_vector);
    return result[0] + result[1] + result[2] + result[3];
}

double integrate8(double* values, std::size_t size, double step) {
    auto sum_vector = _mm512_set_pd(0, 0, 0, 0, 0, 0, 0, 0);
    auto step_vector = _mm512_set_pd(step, step, step, step, step, step, step, step);
    for (std::size_t i = 0; i < size; i += 8) {
        auto value_vector = _mm512_load_pd (values + i);
        value_vector = _mm512_mul_pd(value_vector, step_vector);
        sum_vector = _mm512_add_pd(sum_vector, value_vector);
    }
    alignas(64) double result[8];
    _mm512_store_pd(result, sum_vector);
    return result[0] + result[1] + result[2] + result[3] + result[4] + result[5] + result[6] + result[7];
}


using integration_t = double (*)(double*, std::size_t, double);

double measure(double* values, std::size_t size, double step, integration_t method, std::size_t thread_count) {
    std::vector<std::thread> threads;
    std::vector<double> results;
    results.resize(thread_count);
    volatile bool start = false;
    volatile std::size_t done = 0;

    for (std::size_t i = 0; i < thread_count; i++) {
        threads.emplace_back( [&, i] () {
            while (!start);
            results[i] = method(values + (size / thread_count) * i, size / thread_count, step);
            done += 1;
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    struct timespec start_tm, finish_tm;

    clock_gettime(CLOCK_MONOTONIC, &start_tm);
    start = true;

    while(done != thread_count);

    double res = std::accumulate(results.begin(), results.end(), 0.0);
    clock_gettime(CLOCK_MONOTONIC, &finish_tm);
    for (auto&& thread : threads) {
        thread.join();
    }

    double elapsed = (finish_tm.tv_sec - start_tm.tv_sec);;
    elapsed += (finish_tm.tv_nsec - start_tm.tv_nsec) / 1000000000.0;
    std::cout << "PART SIZE:" << size / thread_count << "\n";
    std::cout << "ELAPSED:" << elapsed << "\n";
    return res;
}
 
}

int main () {
    constexpr std::size_t size = 13 * 3 * 3 * 5 * 5 * 7 * 16 * 11 * 500;
    constexpr double length = 100;
    constexpr double step = length / size;
    auto data = create_data(size, step);
    double ideal = integrate(data.get(), size, step);
    std::cout << "IDEAL:" << ideal << "\n";

    struct {
        std::string method;
        integration_t ptr;
    } methods[] = {{"simd_1", integrate}}; 

    for (auto& method : methods) {
        for (std::size_t i = 1; i <= std::thread::hardware_concurrency(); i++) {
            std::cout << "THREADS:" << i << " METHOD:" << method.method << "\n";
            double res = measure(data.get(), size, step, method.ptr, i);
            if (abs(res - ideal) > 0.1) {
                std::cout << "BAD CALC\n";
            }
            std::cout << res << "\n\n";
        }
    }
 
}

