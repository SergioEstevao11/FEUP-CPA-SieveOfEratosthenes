/*
 *  soeparallel.cpp - implementation of the sieve of erastothenes w/ parallel lookups
 *  CPA @ M.EIC, 2023
 *  Authors:
 *      Miguel Rodrigues <up201906042@edu.fe.up.pt>
 *      Sérgio Estêvão <up201905680@edu.fe.up.pt>
 */
#include <algorithm>
#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <vector>
#include <utility>
#include <omp.h>
#include "soehelpers.h"


static constexpr std::uint64_t N = 4294967296;
static constexpr std::uint64_t L1D_CACHE_SIZE = 32 * 1024 * 8;
static constexpr std::uint8_t NUM_THREADS = 12;

int 
main(void)
{
    cpa::sieve_of_erastothenes::PAPI_helper helper;

    constexpr auto n_sqrt = static_cast<std::uint64_t>(std::sqrt(N));
    constexpr auto segment_size = std::max(n_sqrt, L1D_CACHE_SIZE);

    constexpr auto segment_index = []<typename T>(const T& k, const T& m) {
        return k * k > m ? k * k - m : (2 * k) - ((m - k) % (2 * k));
    };

    std::bitset<segment_size> sieve;

    std::bitset<(n_sqrt >> 1) + 1> is_prime;
    is_prime.set();

    std::uint64_t computed = 0;

    helper.start();
    const auto start = std::chrono::steady_clock::now();

    for (std::uint64_t k = 3; k * k <= N; k += 2) {
        if (!is_prime[k >> 1])
            continue;

        for (auto i = k * k; i <= n_sqrt; i += 2 * k) {
            is_prime[i >> 1] = false;
        }
    }

    #pragma omp parallel for num_threads(NUM_THREADS) private(sieve) reduction(+:computed)
    for (std::uint64_t low = 0; low <= N; low += segment_size) {
        
        const auto high = std::min(N, low + segment_size - 1);
        std::vector<std::pair<std::uint64_t, std::uint64_t>> seeds;

        for (std::uint64_t k = 3; k * k < high; k += 2) {
            if (is_prime[k >> 1]) {
                seeds.emplace_back(k, segment_index(k, low));
            }
        }

        sieve.set();

        for (auto& [seed, i] : seeds) {
            for (const auto incr = 2 * seed; i < segment_size; i += incr) {
                sieve[i] = false;
            }
        }

        for (auto it = low + 1; it <= high; it += 2) {
            computed += sieve[it - low];
        }
    }

    const auto end = std::chrono::steady_clock::now();
    helper.stop();

    const auto elapsed = end - start;
    cpa::sieve_of_erastothenes::report<N>(helper, computed, elapsed);
    
    return 0;
}
