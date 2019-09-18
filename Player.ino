#include "Player.h"
#include <FastLED.h>

/**
   @return true as long as clicked - flags as screen update needed
*/
boolean Player::clickLogic() {
  for (byte i = 0; i < NUM_BLOCKS / 2; i++) {
    this->b[i]->storeInput();

    if (this->b[i]->isTriggered()) {
      if (this->b[i]->equals(colour)) {
        this->score++;
        this->randomizeTarget();
        showLed();
      } else {
        this->score--;
      }
      return true;
    }
  }

  return false;
}

void Player::randomizeTarget() {
  byte target;

  do {
    target = byte(random(0, NUM_BLOCKS / 2));
  } while (target == pTarget);

  if (pTarget != -1) this->b[pTarget]->setColour(CRGB::Black);
  this->b[target]->setColour(colour);

  pTarget = target;
}



void ModeVersus::setupPlayers() {
  delete this->p1;
  delete this->p2;

  this->p1 = new Player(CRGB::Red);
  this->p2 = new Player(CRGB::Blue);

  this->p1->scoreReq = this->stageReq;
  this->p2->scoreReq = this->stageReq;

  for (byte i = 0; i < NUM_BLOCKS / 2; i++) {
    this->p1->b[i] = blocks[P1_BLOCK_INDEX[i]];
    this->p2->b[i] = blocks[P2_BLOCK_INDEX[i]];
  }
}
