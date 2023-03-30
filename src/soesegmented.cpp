/*
 *  soesegmented.cpp - implementation of the segmented sieve of erastothenes
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
#include "soehelpers.h"


static constexpr std::uint64_t N = 536870912;
static constexpr std::uint64_t L1D_CACHE_SIZE = 32 * 1024 * 8;

int 
main(void)
{
    cpa::sieve_of_erastothenes::PAPI_helper helper;

    constexpr auto n_sqrt = static_cast<std::uint64_t>(std::sqrt(N));
    constexpr auto segment_size = std::max(n_sqrt, L1D_CACHE_SIZE);

    std::bitset<segment_size> sieve;

    std::vector<std::pair<std::uint64_t, std::uint64_t>> seeds;
    std::bitset<(n_sqrt >> 1) + 1> is_prime;
    is_prime.set();

    std::uint64_t computed = 0, k = 3;

    helper.start();
    const auto start = std::chrono::steady_clock::now();

    for (std::uint64_t low = 0; low <= N; low += segment_size) {

        const auto high = std::min(N, low + segment_size - 1);

        for (; k * k <= high; k += 2) {
            if (!is_prime[k >> 1])
                continue;
            
            for (std::uint64_t i = k * k; i <= n_sqrt; i += 2 * k) {
                is_prime[i >> 1] = false;
            }
            seeds.emplace_back(k, k * k - low);
        }

        sieve.set();

        for (auto& [seed, i] : seeds) {
            for (const auto incr = 2 * seed; i < segment_size; i += incr) {
                sieve[i] = false;
            }   
            i -= segment_size;
        }

        for (auto i = low + 1; i <= high; i += 2) {
            computed += sieve[i - low];
        }
    }

    const auto end = std::chrono::steady_clock::now();
    helper.stop();

    const auto elapsed = end - start;
    cpa::sieve_of_erastothenes::report<N>(helper, computed, elapsed);

    return 0;
}
