#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <stdint.h>

class Game {
  public:
    void gameBoot();
    void gameLoop();
    
    Arduboy2 arduboy;
    ArduboyTones arSound = ArduboyTones(arduboy.audio.enabled);
  
  private:
    static const unsigned char splash[];
};

