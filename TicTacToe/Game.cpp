// Game.cpp
// Pygasm 2018

#include "Game.h"

void Game::title()
{
  arduboy.boot();

  arduboy.setFrameRate(60);

  arduboy.clear();
}

void Game::game()
{
  if (!arduboy.nextFrame()) {return;}
  arduboy.pollButtons();
  arduboy.clear();

  board.upd(arduboy, boardCells);
  selector.upd(arduboy);

  if (players[0] == 1) {players[0]++;}
  else if (players[0] == 2) {players[0]--;}

  arduboy.display();
}

