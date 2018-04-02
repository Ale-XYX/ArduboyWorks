#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <stdint.h>
#include "classes.h"

class Game {
  public:
    void gameBoot();
    void gameLoop();

    void updateTitle();
    void drawTitle();

    void updateGame();
    void drawGame();
    
    Arduboy2 arduboy;
    ArduboyTones arSound = ArduboyTones(arduboy.audio.enabled);

    

  private:
    static const unsigned char splash[];
};

