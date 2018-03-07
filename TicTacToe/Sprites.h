// Sprites.h
// Pygasm 2018

#pragma once
#include <Arduboy2.h>

class Board
{
  public:
  	int cells[18] = 
  	{
  		0, (0, 0),
  		0, (1, 0),
  		0, (2, 0),
  		0, (0, 1),
  		0, (1, 1),
  		0, (2, 1),
  	  0, (0, 2),
  		0, (1, 2),
  		0, (2, 2)
  	};
  	void upd(Arduboy2 & arduboy);
  
  private:
  	const int pos[2] = {43, 9};
  	const int len[2] = {46, 46};
  	static const unsigned char image[];
};
