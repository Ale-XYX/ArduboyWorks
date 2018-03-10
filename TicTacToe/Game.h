// Game.h
// Pygasm 2018

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "Classes.h"

class Game
{
  public:
    int players[3] = {1, 0, 0};
    int boardCells[3][9][2] =
    {
      {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {0, 2}, {1, 2}, {2, 2}},
      {{0, 0}, {1, 0}, {2, 0}, {2, 0}, {1, 0}, {0, 0}, {0, 0}, {1, 0}, {2, 0}},
      {{43 + 2, 9 + 2}, {59 + 2, 9 + 2}, {75 + 2, 9 + 2}, {43 + 2, 25 + 2}, {59 + 2, 25 + 2}, {75 + 2, 25 + 2}, {43 + 2, 41 + 2}, {59 + 2, 41 + 2}, {75 + 2, 41 + 2}}
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

