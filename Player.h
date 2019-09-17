#ifndef PLAYER_H
#define PLAYER_H

#include "InputHandler.h"

class Player {
  public:
    // Constructor
    Player(CRGB c) {
      colour = c;
    }
  
    Block *b[NUM_BLOCKS/2];
    byte score = 0;
    CRGB colour;

    void addScore();
};



extern Player *p1;
extern Player *p2;

#endif
