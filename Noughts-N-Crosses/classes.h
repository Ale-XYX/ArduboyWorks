#include <stdint.h>
#pragma once

enum class States : uint8_t {
  Title, Game, // Game screens
  
  Main, Paused, Winner // Game states
};

enum class Cell : uint8_t {
  Empty, Cross, Nought
};

class Point2 {
  public:
   int16_t x;
   int16_t y;
  
   Point2(void) = default;
   Point2(int16_t x, int16_t y) : x(x), y(y) {}
};
