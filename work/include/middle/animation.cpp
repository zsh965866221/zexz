#include "middle/animation.h"

#include "glog/logging.h"

#include "zexz/utils/utils.h"

namespace zexz {
namespace middle {
namespace animation {

Timer::Timer(float maxTime):
    time(0.0f),
    currTime(0.0),
    maxTime(maxTime),
    status(AnimationStatus_Stoped),
    interrupted(false) {
}

Timer::Timer():
  Timer(10.0f) {}

void Timer::start() {
  currTime = timer.delta();
  time = 0.0;
  status = AnimationStatus_Runing;
}

void Timer::pause() {
  switch (status)
  {
  case AnimationStatus_Runing:
    status = AnimationStatus_Pausing;
    break;
  case AnimationStatus_Pausing:
    status = AnimationStatus_Runing;
    break;
  case AnimationStatus_Stoped:
    status = AnimationStatus_Stoped;
    break;
  }
}

void Timer::stop() {
  time = 0.0f;
  status = AnimationStatus_Stoped;
}

void Timer::update() {
  float t = timer.delta();
  switch (status)
  {
  case AnimationStatus_Runing:
    time += (t - currTime);
    currTime = t;
    break;
  case AnimationStatus_Pausing:
    break;
  case AnimationStatus_Stoped:
    time = 0.0f;
    break;
  }
  currTime = t;
  if (interrupted == true && time > maxTime - zexz::utils::EPSILON_FLOAT) {
    time = 0.0f;
  }
}

Animation::Animation() {}

} // namespace animation
} // namespace middle
} // namespace zexz