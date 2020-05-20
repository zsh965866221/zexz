#include "middle/animation.h"

#include "glog/logging.h"

#include "zexz/utils/utils.h"

namespace zexz {
namespace middle {

Animation::Animation(float maxTime):
    time(0.0f),
    currTime(0.0),
    maxTime(maxTime),
    status(AnimationStatus_Stoped),
    interrupted(false) {
}

Animation::Animation():
  Animation(10.0f) {}

void Animation::start() {
  currTime = timer.delta();
  time = 0.0;
  status = AnimationStatus_Runing;
}

void Animation::pause() {
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

void Animation::stop() {
  time = 0.0f;
  status = AnimationStatus_Stoped;
}

void Animation::update() {
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
  if (interrupted == true && time > maxTime - utils::EPSILON_FLOAT) {
    time = 0.0f;
  }
}

} // namespace middle
} // namespace zexz