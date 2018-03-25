#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <stdint.h>

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

class Game {
  public:
    byte gstate = 0;
    byte gscreen = 0;
    
    void updateGrid(uint16_t x, uint16_t y);
    void updatePlayerPanel(uint16_t x, uint16_t y);
    void updateInput();
    void updateGameState();
    void updateGameScreen();
    void gameSetup();
    void gameLoop();
    bool isCatsGame();
    
    Arduboy2 arduboy;
    ArduboyTones sound = ArduboyTones(arduboy.audio.enabled);

    Cell player = randomCell();
    Cell winner;
    Cell grid[3][3];
    Cell updateWinners();
    Cell randomCell();
    Point2 selector;
    Point2 winningSets[8][3] = {
      {{0, 0}, {0, 1}, {0, 2}},
      {{1, 0}, {1, 1}, {1, 2}},
      {{2, 0}, {2, 1}, {2, 2}},

      {{0, 0}, {1, 0}, {2, 0}},
      {{0, 1}, {1, 1}, {2, 1}},
      {{0, 2}, {1, 2}, {2, 2}},

      {{0, 0}, {1, 1}, {2, 2}},
      {{0, 2}, {1, 1}, {2, 0}}
    };

  private:
    static const unsigned char logoImage[];

    static const unsigned char titleImage[];
    
    static const unsigned char boardImage[];
    static const unsigned char selectorImage[];
    
    static const unsigned char crossImage[];
    static const unsigned char noughtImage[];
};

