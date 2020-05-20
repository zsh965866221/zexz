#include "middle/animation.h"

#include "glog/logging.h"

#include "zexz/utils/utils.h"

namespace zexz {
namespace middle {


float Animation::now() {
  auto t = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
  return static_cast<float>((t - objectTime).count()) / 1000000.0f;
}

Animation::Animation(float maxTime):
    time(0.0f),
    currTime(now()),
    maxTime(maxTime),
    status(AnimationStatus_Stoped) {
  objectTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
}

Animation::Animation():
  Animation(10.0f) {}

void Animation::start() {
  currTime = now();
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
  float t = now();
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
  if (time > maxTime - utils::EPSILON_FLOAT) {
    time = 0.0f;
  }
}

} // namespace middle
} // namespace zexz