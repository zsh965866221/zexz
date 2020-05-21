#include "middle/timer.h"

namespace zexz {
namespace middle {
namespace utils {

TimePointMicro Timer::now() {
  return std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
}

Timer::Timer():
  objectTime(now()) {}

float Timer::delta() {
  return static_cast<float>((now() - objectTime).count()) / 1000000.0f;
}

} // namespace utils
} // namespace middle
} // namespace zexz