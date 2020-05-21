#pragma once

#include <chrono>

#include "middle/timer.h"

namespace zexz {
namespace middle {

namespace animation {

enum AnimationStatus {
  AnimationStatus_Runing = 0,
  AnimationStatus_Pausing,
  AnimationStatus_Stoped
};

class Timer {
public:
  Timer();
  Timer(float maxTime);

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
  bool interrupted;

private:
  zexz::middle::utils::Timer timer;
};

class Animation {
public:
  Animation();

public:
  Timer timer;
};

} // namespace animation
} // namespace middle
} // namespace zexz