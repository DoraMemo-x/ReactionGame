#include "Game.h"
#include "InputHandler.h"



static const byte BUTTON_PINS[] = {2, 3, 4, 5, 6, 7}; // NOTE: static use: limit scope
static const byte MODE_BUTTON_PIN = A0;

Block *blocks[NUM_BLOCKS];
byte modeInput = 1;
static boolean pModeBtnState = false, modeBtnState = false;



Block::Block(int btn, int led) {
  buttonPin = btn;
  ledIndex = led;
  this->colour = CRGB::Black;
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
  for (byte i = 0; i < NUM_BLOCKS; i++) {
    delete blocks[i];
    
    pinMode(BUTTON_PINS[i], INPUT);
    digitalWrite(BUTTON_PINS[i], HIGH); // turn on pullup resistor (is this how it's done?)

    blocks[i] = new Block(BUTTON_PINS[i], i);
    blocks[i]->setColour(CRGB::Black);
  }

  pinMode(A0, INPUT);
}

void lightUp(CRGB colour) {
  for (byte i = 0; i < NUM_BLOCKS; i++) {
    blocks[i]->setColour(colour);
  }
}

void updateMode() {
  pModeBtnState = modeBtnState;
  modeBtnState = !digitalRead(A0);

  if (modeBtnState && !pModeBtnState) {
    modeInput = (modeInput + 1) % (RETURN);

    lightUp(CRGB::Black);
    setupGame();
    setupMonitor();

    setBeginMillis();
//    if (game->getMode() == Classic || game->getMode() == Debut) updateTime(getSecondsRemaining());

    showLed();
  }
}

//void storeInputAll(Block *blocks[]) {
//  for (int i = 0; i < NUM_BLOCKS; i++) {
//    Block b = *blocks[i];
//
//    storeInput(&b);
//  }
//}
