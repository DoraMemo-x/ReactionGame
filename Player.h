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
    byte pTarget = -1;
    
    byte score = 0;
    byte scoreReq = 255;
    byte wins = 0;
    CRGB colour;

    boolean clickLogic();
    void randomizeTarget();
};



extern Player *p1;
extern Player *p2;

#endif
