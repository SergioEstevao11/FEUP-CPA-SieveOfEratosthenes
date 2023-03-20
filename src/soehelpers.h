/*
 *  soehelpers.h - set of helpers for sieve of eratosthenes
 *  CPA @ M.EIC, 2023
 *  Authors:
 *      Miguel Rodrigues <up201906042@edu.fe.up.pt>
 *      Sérgio Estêvão <up201905680@edu.fe.up.pt>
 */
#ifndef SOEHELPERS_H
#define SOEHELPERS_H

#include <array>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <papi.h>


namespace cpa::sieve_of_erastothenes {

using duration_t = std::chrono::duration<long double>;

struct PAPI_helper {
public:
    PAPI_helper()
    {
        retval = PAPI_library_init(PAPI_VER_CURRENT);
        if (retval != PAPI_VER_CURRENT && retval > 0)
            handle_error("PAPI_library_init");

        retval = PAPI_create_eventset(&eventset);
        if (retval != PAPI_OK)
            handle_error("PAPI_create_eventset");

        retval = PAPI_add_events(eventset, event_codes.data(), static_cast<int>(event_codes.size()));
        if (retval != PAPI_OK)
            handle_error("PAPI_add_events");
    }

    void start()
    {
        retval = PAPI_start(eventset);
        if (retval != PAPI_OK)
            handle_error("PAPI_start");
    }

    void stop()
    {
        retval = PAPI_stop(eventset, values.data());
        if (retval != PAPI_OK)
            handle_error("PAPI_stop");
    }

    void report() const
    {
        std::cout << "PAPI_L1_DCM: " << values[0] << '\n'
                  << "PAPI_L2_DCM: " << values[1] << '\n'
                  << "PAPI_TOT_INS: " << values[2] << '\n'
                  << "PAPI_TOT_CYC: " << values[3] << std::endl;
    }

    ~PAPI_helper()
    {
        retval = PAPI_reset(eventset);
        if (retval != PAPI_OK)
            handle_error("PAPI_reset");

        retval = PAPI_remove_events(eventset, event_codes.data(), static_cast<int>(event_codes.size()));
        if (retval != PAPI_OK)
            handle_error("PAPI_remove_events");

        retval = PAPI_destroy_eventset(&eventset);
        if (retval != PAPI_OK)
            handle_error("PAPI_destroy_eventeset");
    }

private:
    void handle_error(std::string_view callable) const
    {
        std::cerr << callable << " failed with error "
                  << retval << ' '
                  << '(' << PAPI_strerror(retval) << ')' << std::endl;
        std::exit(1);
    }

private:
    std::array<int, 4> event_codes = { PAPI_L1_DCM, PAPI_L2_DCM, PAPI_TOT_INS, PAPI_TOT_CYC };
    std::array<long long, 4> values = { 0, 0, 0, 0 };
    int eventset = PAPI_NULL;
    int retval = 0;
};


template<std::uint64_t N>
void
report(const cpa::sieve_of_erastothenes::PAPI_helper& helper,
       const std::uint64_t computed,
       const duration_t& elapsed)
{
    constexpr auto OPS = static_cast<std::uint64_t>(N * std::log(std::log(N)));
    std::cout << "limit: " << N << '\n'
              << "operations: " << OPS << '\n'
              << "pi(x): " << computed << '\n'
              << "duration: " << elapsed.count() << 's' << std::endl;

    helper.report();
}

}

#endif /* SOEHELPERS_H */