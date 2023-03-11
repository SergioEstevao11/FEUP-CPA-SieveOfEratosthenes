/*
 *  soebasic.cpp - basic implementation of the sieve of erastothenes
 *  CPA @ M.EIC, 2023
 *  Authors:
 *      Miguel Rodrigues <up201906042@edu.fe.up.pt>
 *      Sérgio Estêvão <up201905680@edu.fe.up.pt>
 */
#include <cstdint>
#include <chrono>
#include <vector>
#include "soehelpers.h"

static constexpr std::uint64_t N = 1 << 25;

int
main(void)
{
    cpa::sieve_of_erastothenes::PAPI_helper helper;

    std::vector<bool> primes(N >> 1);
    std::uint64_t k = 3;

    helper.start();
    const auto start = std::chrono::steady_clock::now();
    
    do {

        for (std::uint64_t i = k * k; i < N; i += 2 * k) {
            if (i % k == 0)
                primes[i >> 1] = true;
        }

        do {
            k += 2;
        } while (k * k <= N && primes[k >> 1]);
        
    } while (k * k <= N);

    const auto end = std::chrono::steady_clock::now();
    helper.stop();
    
    const auto elapsed = end - start;
    cpa::sieve_of_erastothenes::report(helper, elapsed, N);
    cpa::sieve_of_erastothenes::print(primes, N, std::cerr);

    return 0;
}
