/*
 *  soecachewise.cpp - implementation of the sieve of erastothenes w/ lookups
 *  CPA @ M.EIC, 2023
 *  Authors:
 *      Miguel Rodrigues <up201906042@edu.fe.up.pt>
 *      Sérgio Estêvão <up201905680@edu.fe.up.pt>
 */
#include <cstdint>
#include <chrono>
#include <cmath>
#include <vector>
#include "soehelpers.h"


static constexpr std::uint64_t N = 100'000'000'000;
static constexpr std::uint64_t L1D_CACHE_SIZE = 32 * 1024 * 8;

int 
main(void)
{
    cpa::sieve_of_erastothenes::PAPI_helper helper;

    std::uint64_t computed = 1;
    std::uint64_t k = 3, it = 3;

    constexpr std::uint64_t n_sqrt = static_cast<std::uint64_t>(std::sqrt(N));
    constexpr std::uint64_t segment_size = std::max(n_sqrt, L1D_CACHE_SIZE);

    std::vector<bool> sieve(segment_size);
    std::vector<std::pair<std::uint64_t, std::uint64_t>> sieving_primes;
    std::vector<bool> is_prime((n_sqrt >> 1) + 1, true);

    helper.start();
    const auto start = std::chrono::steady_clock::now();

    for (std::uint64_t low = 0; low <= N; low += segment_size) {

        const std::uint64_t high = std::min(N, low + segment_size - 1);

        /* find the primes upto sqrt(segment limit) and compute the indexes */
        for (; k * k <= high; k += 2) {
            if (is_prime[k >> 1]) {
                sieving_primes.emplace_back(k, k * k - low);
                for (std::uint64_t i = k * k; i <= n_sqrt; i += 2 * k) {
                    is_prime[i >> 1] = false;
                }
            }
        }

        std::fill(sieve.begin(), sieve.end(), true);

        for (auto& [prime, multiple] : sieving_primes) {
            std::uint64_t i = multiple;
            for (std::uint64_t j = prime * 2; i < segment_size; i += j) {
                sieve[i] = false;
            }
            multiple = i - segment_size;
        }

        /* Check the number of computed primes */
        for (; it <= high; it += 2) {
            computed += sieve[it - low];
        }
    }

    const auto end = std::chrono::steady_clock::now();
    helper.stop();

    const auto elapsed = end - start;
    cpa::sieve_of_erastothenes::report(helper, N, computed, elapsed);

    return 0;
}
