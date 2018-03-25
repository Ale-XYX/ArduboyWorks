// Noughts-N-Crosses
// Note: Sorry I dont comment my code, It just becomes too verbose and clutters my code even further ¯\_(ツ)_/¯

#include "Game.h"

Game game;

void setup() {
  Serial.begin(9600);
  
  game.gameSetup();
}

void loop() {
  game.gameLoop();
}

// Pygasm 2018

