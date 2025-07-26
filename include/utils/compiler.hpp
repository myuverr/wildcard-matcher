#pragma once

#include <cassert>
#include <cstdlib>

/**
 * @brief Notifies the compiler that a point in the code is unreachable.
 */
#if defined(__GNUC__) || defined(__clang__)
#define APP_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define APP_UNREACHABLE() __assume(0)
#else
// A robust fallback for unsupported compilers.
#define APP_UNREACHABLE()                                          \
    do {                                                           \
        assert(false && "Fatal: Unreachable code path executed."); \
        std::abort();                                              \
    } while (0)
#endif