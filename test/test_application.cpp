#include <iostream>
#include <string>

#include "middle/application.h"

using namespace zexz::middle;

int main(int, char**) {
  Application app;
  app.init();
  app.run();
  return 0;
}