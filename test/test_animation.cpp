#include "middle/animation.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace zexz::middle::animation;

int main() {
  Timer animation;
  animation.start();
  for (int i = 0; i < 100; i++) {
    animation.update();
    std::cout << animation.time << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    if (i == 50) {
      animation.pause();
    }
    if (i == 80) {
      animation.pause();
    }
  }

}