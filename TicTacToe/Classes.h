// Sprites.h
// Pygasm 2018

#pragma once
#include <Arduboy2.h>
#include <ArduboyTones.h>

// Board
class Board
{
  public:
  	void upd(Arduboy2 & arduboy, int cells[3][9][2]);
  
  private:
  	const int pos[2] = {43, 9};
  	const int len[2] = {46, 46};
  	static const unsigned char image[];
    static const unsigned char x[];
    static const unsigned char o[];
};


// Selector
class Selector
{
  public:
    void upd(Arduboy2 & arduboy, ArduboyTones & sound);
    int posX[3] = {43, 59, 75};
    int posY[3] = {9, 25, 41};
    int len[2] = {15, 15};
    
    int indexX = 0;
    int indexY = 0;
    
  private:
    static const unsigned char image[];
};

