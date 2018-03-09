// TicTacToe
// Pygasm 2018

#include "Game.h"
#include <ArduboyTones.h>

Game game;

void setup() 
{
  game.title();
}

ArduboyTones sound(game.arduboy.audio.enabled); // arduino wont let me define this in the game class because arduboy was not initizalized and I could not define it in title because it was in a function scope and I could not have one class and then override it with the correct one because that deleted Game::game so I gave up and did this ugly solution i hate c++

void loop() {
  game.main(sound);
}
