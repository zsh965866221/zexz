#pragma once

#include <chrono>

namespace zexz {
namespace middle {
namespace utils {

typedef std::chrono::time_point<std::chrono::system_clock,std::chrono::microseconds> TimePointMicro;

class Timer {
public:
  Timer();

public:
  static TimePointMicro now();
  float delta();

private:
  TimePointMicro objectTime;
};

} //namespace utils
} // namespace middle
} // namespace zexz