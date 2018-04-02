#include "game.h"
#include "classes.h"

// Functions
void Game::gameBoot() {
  Serial.begin(9600); // Begin Screen Mirroring
  arduboy.boot(); // So I can use my custom splash
  arduboy.audio.begin();
  
  selector.x = 1;
  selector.y = 1;

  arSound.tone(200, 150, 300, 150, 250, 200);
  arduboy.drawBitmap(12, 23, splash, 103, 17);
  arduboy.display();
  
  delay(1500);

  arduboy.clear();
}

void Game::gameLoop() {
  if (!arduboy.nextFrame()) {return;}
  arduboy.pollButtons();
  
  arduboy.clear();

  if (gameScreen == States::Title) {
    titleLogic();
    titleDraw();
  } else if (gameScreen == States::Game) {
    gameLogic();
    gameDraw();
  }
  
  Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
  arduboy.display();
}

void Game::titleLogic() {
  if (arduboy.justPressed(A_BUTTON)) {
    gameScreen = States::Game;
    arSound.tone(200, 100, 300, 100);
  }
}

void Game::titleDraw() {
  arduboy.setCursor(35, 50);

  arduboy.drawBitmap(23, 0, title, 81, 42);
  arduboy.print(F("A To Start"));
}

void Game::gameLogic() {
  if (arduboy.justPressed(RIGHT_BUTTON) and gameState == States::Main) {
    selector.x++;
    arSound.tone(300, 75);
    if (selector.x > 2) {selector.x = 0;}
  } else if (arduboy.justPressed(LEFT_BUTTON) and gameState == States::Main) {
    selector.x--;
    arSound.tone(300, 75);
    if (selector.x < 0) {selector.x = 2;}
  } else if (arduboy.justPressed(UP_BUTTON) and gameState == States::Main) {
    selector.y--;
    arSound.tone(300, 75);
    if (selector.y < 0) {selector.y = 2;}
  } else if (arduboy.justPressed(DOWN_BUTTON) and gameState == States::Main) {
    selector.y++;
    arSound.tone(300, 75);
    if (selector.y > 2) {selector.y = 0;}
  }


  if (arduboy.justPressed(A_BUTTON) and gameState == States::Main) {
    if (grid[selector.y][selector.x] == Cell::Empty) {
      grid[selector.y][selector.x] = player;
      player = (player == Cell::Nought) ? Cell::Cross : Cell::Nought; // If player is nought, eval to cross, else eval to nought
      
      Cell winrar = winCheck();
      if (winrar == Cell::Empty) {
        if (drawCheck()) {
          arSound.tone(400, 100, 300, 100, 200, 100);
          gameState = States::Winner;
        } else {arSound.tone(200, 100, 150, 100);}
      } else {
        arSound.tone(200, 100, 300, 100, 400, 100);
        
        gameState = States::Winner;
        winner = winrar;
      }
    } else {
      arSound.tone(100, 100);
    }
  }

  if (arduboy.justPressed(B_BUTTON) and gameState == States::Main) {
    arSound.tone(400, 100, 500, 100);
    gameState = States::Paused;
  } else if (arduboy.justPressed(B_BUTTON) and gameState == States::Paused) {
    arSound.tone(500, 100, 400, 100);
    gameState = States::Main;
  } else if (arduboy.justPressed(B_BUTTON) and gameState == States::Winner) {
    for (uint8_t y = 0; y < 3; y++) {
      for (uint8_t x = 0; x < 3; x++) {
        grid[y][x] = Cell::Empty;
      }
    }
    gameScreen = States::Title;
    gameState = States::Main;
    player = Cell::Cross;
    winner = Cell::Empty;
    selector.x = 1;
    selector.y = 1;
    
    arSound.tone(500, 100);
  }
  
}

void Game::gameDraw() {
  // Board Drawing
  arduboy.drawBitmap(64, 1, board, 61, 61);
  
  for (uint8_t yi = 0; yi < 3; ++yi) {
    for (uint8_t xi = 0; xi < 3; ++xi) {
      uint16_t yPos = 1 + (yi * 21);
      uint16_t xPos = 64 + (xi * 21);

      if (grid[yi][xi] == Cell::Nought) {
        arduboy.drawBitmap(xPos + 3, yPos + 3, nought, 13, 13);
      } else if (grid[yi][xi] == Cell::Cross) {
        arduboy.drawBitmap(xPos + 3, yPos + 3, cross, 13, 13);
      }

      if(xi == selector.x && yi == selector.y) {
        arduboy.drawBitmap(xPos + 1, yPos + 1, selector_, 17, 17);
      }
    }
  }

  // Player Panel Drawing
  for (uint8_t yi = 0; yi < 2; yi++) {
    
    // Set y position
    uint16_t yPos = 2 + (yi * 15);

    arduboy.setCursor(20, yPos + 2);

    arduboy.print(F("P"));
    arduboy.print(yi + 1);

    arduboy.drawRect(5, yPos, 10, 10);
    if(static_cast<uint8_t>(player) - 1 == yi) {arduboy.fillRect(7, yPos + 2, 6, 6);}
  }

  // Status drawing
  arduboy.setCursor(5, 35);
  if (gameState == States::Main) {
    return;
  } else if (gameState == States::Paused) {
    arduboy.print("Paused.");
  } else if (gameState == States::Winner) {
    arduboy.print((winner == Cell::Cross) ? F("Crosses") : (winner == Cell::Nought) ? F("Noughts") : F("Draw!"));
    arduboy.setCursor(5, 45);
    arduboy.print((winner == Cell::Empty) ? F(" ") : F("Wins!"));
  }
}

bool Game::drawCheck() {
  for (uint8_t y = 0; y < 3; y++) {
    for (uint8_t x = 0; x < 3; x++) {
      if (grid[y][x] == Cell::Empty) {
        return false;
      }
    }
  }
  return true;
}

Cell Game::winCheck() {
  for (uint8_t i = 0; i < 8; i++) {
    Point2 a = winningSets[i][0];
    Point2 b = winningSets[i][1];
    Point2 c = winningSets[i][2];
    Cell target = grid[a.y][a.x];
    
    if (target != Cell::Empty && grid[b.y][b.x] == target && grid[c.y][c.x] == target) {
      return target;
    }
  }
  return Cell::Empty;
}

// Images
const unsigned char Game::splash[] PROGMEM = {
  0x38, 0x48, 0x88, 0x8, 0x4, 0x2, 0x1, 0xc1, 0x1, 0x1, 0x1, 0x2, 0x4, 0x38, 0x20, 0x20, 0x10, 0x70, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc7, 0xc7, 0xc7, 0xff, 
  0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7, 0x7, 0x7, 0x87, 0x87, 0x87, 0x87, 0x87, 0x87, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0xc7, 0xc7, 0xc7, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0xc7, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x7, 0x3f, 0x3f, 
  0xf8, 0xc0, 0xf8, 0x3f, 0x3f, 0x7, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1, 0xd, 0x7a, 0x82, 0x00, 0x80, 0x80, 0x00, 0x80, 0x80, 0x40, 0x20, 0x10, 0x8, 0x8, 0x4, 0x3, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0xff, 0xff, 0xff, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xc0, 0xc0, 0xc0, 0xc3,
  0xc3, 0xc3, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x1, 0x1, 0x1, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xff, 0xff, 0xff, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x00, 0x1, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 
  0x1, 0x1, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x1, 0x1
};

const unsigned char Game::title[] PROGMEM = {
  0xfe, 0xff, 0xff, 0x7, 0x7, 0x7, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x7, 0x7, 0x7, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xfe, 
  0xff, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x7, 0x7, 0x47, 0xe7, 0xe7, 0xc2, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xfe, 0x70, 0x70, 0x70, 0xfe, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x2, 0x7, 
  0x7, 0xff, 0xff, 0xff, 0x7, 0x7, 0x2, 0x00, 0x00, 0x00, 0x3e, 0x7f, 0x7f, 0x77, 0x77, 0x77, 0xf7, 0xf7, 0xe2, 0x7, 0xf, 0x7, 0x00, 0x00, 0x00, 0x7, 0xf, 0x7, 0x00, 0x00, 0x00, 0x7, 
  0xf, 0xf, 0xe, 0xe, 0xe, 0xf, 0xf, 0x7, 0x00, 0x00, 0x00, 0x7, 0xf, 0xf, 0xe, 0xe, 0xe, 0xf, 0xf, 0x7, 0x00, 0x00, 0x00, 0x7, 0x8f, 0x8f, 0x8e, 0x8e, 0x8e, 0x8f, 0x8f, 0x7, 0x00, 0x00, 
  0x00, 0x7, 0xf, 0x7, 0x00, 0x00, 0x00, 0x7, 0xf, 0x7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7, 0xf, 0x7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4, 0xe, 0xe, 0xe, 0xe, 0xe, 0xf, 0xf, 0x7, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x3, 0x3, 0x3, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 
  0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x83, 0xc7, 0xc3, 0xc0, 0xc0, 0xc0, 0xc3, 
  0xc7, 0x83, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xc0, 
  0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0xff, 0xff, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x1, 0x1, 0x1, 0xf, 0x1f, 0xf, 0x00, 0x00, 0x00, 0xff, 
  0xff, 0xff, 0x81, 0x81, 0x81, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xf, 0x9f, 0x9f, 0x9d, 0x9d, 0x9d, 0xfd, 0xfd, 0xf8, 0x00, 0x00, 0x00, 0xf, 0x9f, 0x9f, 0x9d, 0x9d, 0x9d, 0xfd, 0xfd, 
  0xf8, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x9d, 0x9d, 0x9d, 0x9d, 0x89, 0x00, 0x00, 0x00, 0x00, 0xf, 0x9f, 0x9f, 0x9d, 0x9d, 0x9d, 0xfd, 0xfd, 0xf8, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
  0x3, 0x1, 0x00, 0x00, 0x00, 0x1, 0x3, 0x3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x1, 0x00, 0x00, 0x00, 0x1, 0x3, 0x3, 0x3, 0x3, 
  0x3, 0x3, 0x3, 0x1, 0x00, 0x00, 0x00, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x1, 0x00, 0x00, 0x00, 0x1, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x1, 0x00, 0x00, 0x00, 0x1, 0x3, 0x3, 0x3, 
  0x3, 0x3, 0x3, 0x3, 0x1, 
};

const unsigned char Game::board[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 
  0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xff, 0xff, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
  0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xff, 0xff, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  
};

const unsigned char Game::cross[] PROGMEM = {
  0x3, 0x3, 0xc, 0xc, 0xb0, 0xf0, 0xe0, 0xf0, 0xb0, 0xc, 0xc, 0x3, 0x3, 0x18, 0x18, 0x6, 0x6, 0x1, 0x1, 0x00, 0x1, 0x1, 0x6, 0x6, 0x18, 0x18
};

const unsigned char Game::nought[] PROGMEM = {
  0xfc, 0xfc, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0xfc, 0xfc, 0x7, 0x7, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7, 0x7
};

const unsigned char Game::selector_[] PROGMEM = {
  0xff, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xff, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 
};
