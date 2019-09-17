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

void loop() {
  updateMode();

  // Only applicable when not game over
  if (!game->isGameOver()) {
    game->clickLogic();
    game->updateState();

    game->updateScreenTimeRemaining();
  }
}



//bool operator==(const CRGB &a, const CRGB &b) {
//  return a.red == b.red && a.green == b.green && a.blue == b.blue;
//}
