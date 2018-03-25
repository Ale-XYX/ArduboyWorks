// Main game

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

