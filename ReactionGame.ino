#include "BoardHandler.h"
#include "LED.h"
#include "Game.h"
#include "LCDMonitor.h"

void setup() {
  Serial.begin(9600);

  setupLed();
  setupBoard();

  determineGameMode();
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

    if (game->periodicTimer(1000)) {
      game->periodicEvent();
    }
  }
}



//bool operator==(const CRGB &a, const CRGB &b) {
//  return a.red == b.red && a.green == b.green && a.blue == b.blue;
//}
