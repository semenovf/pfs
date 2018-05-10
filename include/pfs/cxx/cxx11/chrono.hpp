#pragma once
#include <chrono>

namespace pfs {
namespace chrono {

template <typename Clock, typename Duration = typename Clock::duration>
using time_point = std::chrono::time_point<Clock, Duration>;

template <typename Rep, typename Period = std::ratio<1>>
using duration = std::chrono::duration<Rep, Period>;

typedef std::chrono::nanoseconds  nanoseconds;
typedef std::chrono::microseconds microseconds;
typedef std::chrono::milliseconds milliseconds;
typedef std::chrono::seconds      seconds;
typedef std::chrono::minutes      minutes;
typedef std::chrono::hours        hours;

}} // pfs::chrono
