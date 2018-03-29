#include "game.h"

Game game;

void setup() {
  Serial.begin(9600);
  
  game.gameSetup();
}

void loop() {
  game.gameLoop();
}
