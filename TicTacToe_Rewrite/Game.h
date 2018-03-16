#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <stdint.h>

enum class Cell : uint8_t {
  Empty, Nought, Cross
};

class Point2 {
  public:
   int16_t x;
   int16_t y;
  
   Point2(void) = default;
   Point2(int16_t x, int16_t y) : x(x), y(y) {}
};

class Game {
  public:
    Cell grid[3][3];
    Point2 selector;
    
    void drawGridAt(uint16_t x, uint16_t y);
    void gameSetup();
    void gameLoop();
    
    Arduboy2 arduboy;
    ArduboyTones sound = ArduboyTones(arduboy.audio.enabled);

  private:
    static const unsigned char logoImage[];
    
    static const unsigned char boardImage[];
    static const unsigned char selectorImage[];
    
    static const unsigned char crossImage[];
    static const unsigned char noughtImage[];
};

