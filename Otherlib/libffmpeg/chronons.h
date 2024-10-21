#pragma once

#include <chrono>

inline constexpr int64_t operator"" _i64(unsigned long long int n) noexcept {
    return static_cast<int64_t>(n);
}
using fixed32 = std::chrono::duration<int64_t, std::ratio<1, (1_i64 << 32)>>;
using nanoseconds = std::chrono::nanoseconds;
using microseconds = std::chrono::microseconds;
using milliseconds = std::chrono::milliseconds;
using seconds = std::chrono::seconds;
using seconds_d64 = std::chrono::duration<double>;
