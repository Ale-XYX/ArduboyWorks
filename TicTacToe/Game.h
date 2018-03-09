// Game.h
// Pygasm 2018

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "Classes.h"

class Game
{
  public:
    int boardCells[3][9] = 
    {
      {(0, 0), (1, 0), (2, 0), (0, 1), (1, 1), (2, 1), (0, 2), (1, 2), (2, 2)},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {(43, 9), (59, 9), (75, 9), (43, 25), (59, 25), (75, 25), (43, 41), (59, 41), (75, 9)}
    };
    int players[3] = {1, 0, 0};
    
    void title();
    void main();
    
    Arduboy2 arduboy;
    ArduboyTones tones(arduboy.audio.enabled);
    Board board;
    Selector selector;
    
  private:
    static const unsigned char logo[];
};

