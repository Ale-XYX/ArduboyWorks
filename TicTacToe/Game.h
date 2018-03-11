// Game.h
// Pygasm 2018

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "Classes.h"

class Game
{
  public:
    int player = 1;
    int winner = 0;
    int boardCells[3][9][2] =
    {
      {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {0, 2}, {1, 2}, {2, 2}},
      {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
      {{45, 11}, {61, 11}, {77, 11}, {45, 27}, {61, 27}, {77, 27}, {45, 43}, {61, 43}, {77, 43}}
    };
    
    void title();
    void main();
    
    Arduboy2 arduboy;
    ArduboyTones sound = ArduboyTones(arduboy.audio.enabled);
    Board board;
    Selector selector;
    
  private:
    static const unsigned char logo[];
};

