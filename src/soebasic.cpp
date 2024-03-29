/*
 *  soebasic.cpp - basic implementation of the sieve of erastothenes
 *  CPA @ M.EIC, 2023
 *  Authors:
 *      Miguel Rodrigues <up201906042@edu.fe.up.pt>
 *      Sérgio Estêvão <up201905680@edu.fe.up.pt>
 */
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <vector>
#include "soehelpers.h"

/* It's probably not a good idea to test this for 2^32! */
static constexpr std::uint64_t N = 536870912;

int
main(void)
{
    cpa::sieve_of_erastothenes::PAPI_helper helper;

    std::vector<bool> sieve(N >> 1, true);
    std::uint64_t k = 3;

    helper.start();
    const auto start = std::chrono::steady_clock::now();
    
    do {

        for (std::uint64_t i = k * k; i < N; i += 2) {
            sieve[i >> 1] = sieve[i >> 1] && (i % k != 0);
        }

        do {
            k += 2;
        } while (k * k <= N && !sieve[k >> 1]);
        
    } while (k * k <= N);

    const auto end = std::chrono::steady_clock::now();
    helper.stop();

    const auto elapsed = end - start;
    const auto computed = std::count(sieve.cbegin(), sieve.cend(), true);
    cpa::sieve_of_erastothenes::report<N>(helper, computed, elapsed);

    return 0;
}
