#include "Player.h"
#include <FastLED.h>

Player *p1 = new Player(CRGB::Red);
Player *p2 = new Player(CRGB::Blue);

void Player::addScore() {
  for (byte i = 0; i < NUM_BLOCKS/2; i++) {
    if (this->b[i]->isTriggered()) {
      this->score++;
    }
  }
}



void setupPlayer() {
  for (byte i = 0; i < NUM_BLOCKS/2; i++) {
    p1->b[i] = blocks[i];
  }

  for (byte i = NUM_BLOCKS/2; i < NUM_BLOCKS; i++) {
    p2->b[i - NUM_BLOCKS/2] = blocks[i];
  }
}
