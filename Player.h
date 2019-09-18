#ifndef PLAYER_H
#define PLAYER_H

#include "InputHandler.h"

static const byte P1_BLOCK_INDEXES[] = {0, 1, 5};
static const byte P2_BLOCK_INDEXES[] = {2, 3, 4};

class Player {
  public:
    // Constructor
    Player(CRGB c) {
      colour = c;
    }
  
    byte blockIndexes[NUM_BLOCKS/2];
    byte pTarget = -1;
    
    byte score = 0;
    byte scoreReq = 255;
    byte wins = 0;
    CRGB colour;

    boolean clickLogic();
    void randomizeTarget();
};

#endif
