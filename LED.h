#ifndef LED_H
#define LED_H

#include <FastLED.h>
#include "BoardHandler.h"

#define DATA_PIN 8 // where the (strip of) LEDs is connected to

void setupLed();
void showLed();

extern CRGB leds[NUM_BLOCKS];

#endif
