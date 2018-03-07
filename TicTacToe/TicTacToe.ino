// TicTacToe
// Pygasm 2018

#include <Arduboy2.h>
#include "Sprites.h"
#include "Public.h"

Arduboy2 arduboy;
Board board;

void setup() {
  arduboy.begin();
  
  arduboy.setFrameRate(fps);
  
  arduboy.clear();
}

void loop() {
  if (!arduboy.nextFrame()) {return;}
  arduboy.pollButtons();
  arduboy.clear();

  board.upd(arduboy);

  arduboy.display();
}
