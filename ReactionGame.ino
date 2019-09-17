#include "InputHandler.h"
#include "LED.h"
#include "Game.h"
#include "Monitor.h"

void setup() {
  Serial.begin(9600);

  setupLed();
  setupInput();
  
  setupGame();
  setupMonitor();

  setBeginMillis();
  updateTime(getSecondsRemaining());

  showLed();
}

long timer = 0;
boolean doPrint = false;

void loop() {
  updateMode();

  // Only applicable when not game over
  if (!game->isGameOver()) {
    game->clickLogic();
    game->updateState();

    if (millis() - timer > 1000) {
      timer = millis();
      doPrint = true;
      updateTime(getSecondsRemaining());
    } else doPrint = false;
  }
}



//bool operator==(const CRGB &a, const CRGB &b) {
//  return a.red == b.red && a.green == b.green && a.blue == b.blue;
//}
