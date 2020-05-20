#pragma once

#include <chrono>

#include "middle/timer.h"

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
  bool interrupted;

private:
  Timer timer;
};

} // namespace middle
} // namespace zexz