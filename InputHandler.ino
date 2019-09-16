#include "Game.h"
#include "InputHandler.h"



static const byte BUTTON_PINS[] = {2, 3, 4, 5, 6, 7}; // NOTE: static use: limit scope
static const byte MODE_BUTTON_PIN = A0;

Block blocks[NUM_BLOCKS];
byte modeInput = 0;
static boolean pModeBtnState = false, modeBtnState = false;



Block::Block() {}
Block::Block(int btn, int led) {
  buttonPin = btn;
  ledIndex = led;
}
boolean Block::isTriggered() {
  return this->btnState && !this->pBtnState;
}
/**
   Sets the colour of the led that belongs to this block.
   NOTE: Think of setColour() as setting the state of led. Hence it belongs in the Game tab
   @param colour Specified colour
*/
void Block::setColour(CRGB colour) {
  this->colour = colour;
  leds[ledIndex] = colour;
}

boolean Block::equals(CRGB colour) {
  CRGB c = this->colour;
  return c.r == colour.r && c.g == colour.g && c.b == colour.b;
}

void Block::storeInput() {
  this->pBtnState = this->btnState;
  this->btnState = !digitalRead(this->getButtonPin()); // Note: digitalRead returns TRUE on LOW.
}

int Block::getButtonPin() {
  return buttonPin;
}



void setupInput() {
  for (int i = 0; i < NUM_BLOCKS; i++) {
    pinMode(BUTTON_PINS[i], INPUT);
    digitalWrite(BUTTON_PINS[i], HIGH); // turn on pullup resistor (is this how it's done?)

    blocks[i] = Block(BUTTON_PINS[i], i);
  }

  pinMode(A0, INPUT);
}

void updateMode() {
  pModeBtnState = modeBtnState;
  modeBtnState = !digitalRead(A0);

  if (modeBtnState && !pModeBtnState) {
        modeInput = (modeInput + 1) % (sizeof(Mode) / sizeof(int));
    setupGame();
//    setupMonitor();
//
//    setBeginMillis();
//    updateTime(getSecondsRemaining());
//
//    showLed();
  }
}

//void storeInputAll(Block *blocks[]) {
//  for (int i = 0; i < NUM_BLOCKS; i++) {
//    Block b = *blocks[i];
//
//    storeInput(&b);
//  }
//}
