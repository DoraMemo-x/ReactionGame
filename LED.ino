#include "LED.h"
#include "InputHandler.h"

CRGB leds[NUM_BLOCKS];

void setupLed() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_BLOCKS); 
}

void showLed() {
  FastLED.show();
}
