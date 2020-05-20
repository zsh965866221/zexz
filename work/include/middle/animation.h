#pragma once

#include <chrono>

namespace zexz {
namespace middle {

enum AnimationStatus {
  AnimationStatus_Runing = 0,
  AnimationStatus_Pausing,
  AnimationStatus_Stoped
};

class Animation {
public:
  Animation();
  Animation(float maxTime);

public:
  void start();
  void pause();
  void update();
  void stop();
public:
  float time;
  float maxTime;
  float currTime;
  AnimationStatus status;

private:
  typedef std::chrono::time_point<std::chrono::system_clock,std::chrono::microseconds> TimePoint;
  TimePoint objectTime;
  float now();
};

} // namespace middle
} // namespace zexz