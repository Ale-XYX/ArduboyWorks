#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "classes.h"

class Game {
  public:
    void gameBoot();
    void gameLoop();

    void titleLogic();
    void titleDraw();

    void gameLogic();
    void gameDraw();

    bool drawCheck();
    Cell winCheck();

    Arduboy2 arduboy;
    ArduboyTones arSound = ArduboyTones(arduboy.audio.enabled);

    Cell player = Cell::Cross;
    Cell winner;
    Cell grid[3][3];

    States gameState = States::Main;
    States gameScreen;

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
    static const unsigned char splash[];

    static const unsigned char title[];

    static const unsigned char board[];
    
    static const unsigned char cross[];
    static const unsigned char nought[];
    static const unsigned char selector_[];
};

