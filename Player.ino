#include "Player.h"
#include <FastLED.h>

/**
   @return true as long as clicked - flags as screen update needed
*/
boolean Player::clickLogic() {
  for (byte i = 0; i < NUM_BLOCKS / 2; i++) {
    Block *b = blocks[blockIndexes[i]];
    
    b->storeInput();

    if (b->isTriggered()) {
      if (b->equals(colour)) {
        this->score++;
        this->randomizeTarget();
//        showLed();
      } else {
        score = max(score-1, 0);
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

  if (pTarget != -1) blocks[blockIndexes[pTarget]]->setColour(CRGB::Black);
  blocks[blockIndexes[target]]->setColour(colour);

  showLed();

  pTarget = target;
}



void ModeVersus::setupPlayers() {
  delete p1;
  delete p2;

  p1 = new Player(CRGB::Red);
  p2 = new Player(CRGB::Blue);

  p1->scoreReq = stageReq;
  p2->scoreReq = stageReq;

  for (byte i = 0; i < NUM_BLOCKS / 2; i++) {
    p1->blockIndexes[i] = P1_BLOCK_INDEXES[i];
    p2->blockIndexes[i] = P2_BLOCK_INDEXES[i];
  }
}
