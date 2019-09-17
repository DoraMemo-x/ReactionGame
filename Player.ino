#include "Player.h"
#include <FastLED.h>

void Player::clickLogic() {
  for (byte i = 0; i < NUM_BLOCKS/2; i++) {
    if (this->b[i]->isTriggered()) {
      if (this->b[i]->equals(this->colour)) {
        this->score++;
      } else {
        this->score--;
      }
    }
  }
}



void ModeVersus::setupPlayers() {
  delete this->p1;
  delete this->p2;

  this->p1 = new Player(CRGB::Red);
  this->p2 = new Player(CRGB::Blue);
  
  for (byte i = 0; i < NUM_BLOCKS/2; i++) {
    this->p1->b[i] = blocks[i];
  }

  for (byte i = NUM_BLOCKS/2; i < NUM_BLOCKS; i++) {
    this->p2->b[i - NUM_BLOCKS/2] = blocks[i];
  }
}
