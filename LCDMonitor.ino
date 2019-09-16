#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Monitor.h"
#include "Game.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setupMonitor() {
  lcd.init();
  lcd.backlight();

  switch (gm->getMode()) {
//    default:
    case Classic:
      lcd.setCursor(5, 0);
      lcd.print("Classic");
      break;

    ////////////////////////////

    case Debut:
      lcd.setCursor(6, 0);
      lcd.print("Debut");
      break;

    ////////////////////////////

    case Versus:
      lcd.setCursor(6, 0);
      lcd.print("Versus");
      break;

    ////////////////////////////

    case Colours:
      lcd.setCursor(5, 0);
      lcd.print("Colours");
      break;
  }

  lcd.setCursor(6, 1);
  lcd.print("Mode");

  delay(1500);

  countdown();
  updateMonitor();
}

void updateMonitor() {
  updateStage(gm->stage, getSecondsRemaining());
  updateScore(gm->score, gm->stageReq);
}

void updateTime(int timeRemain) {
  lcd.setCursor(10, 0);
  lcd.print("        ");
  lcd.setCursor(10, 0);
  lcd.print(timeRemain + 1);
  lcd.print(" s)");
}

static void updateStage(int stage, int timeRemain) {
  clearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("Stage: ");
  lcd.print(stage + 1);
  lcd.print(" (");
  lcd.print(timeRemain + 1);
  lcd.print(" s)");
}

static void updateScore(int score, int req) {
  clearLine(1);
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.print(" / ");
  lcd.print(req);
}

static void clearLine(int i) {
  lcd.setCursor(0, i);
  lcd.print("                ");
}

//static int countdownTimer = 3000;
void countdown() {
  clearLine(0);
  clearLine(1);
  lcd.setCursor(5, 0);
  lcd.print("Ready?");
  lcd.setCursor(7, 1);
  lcd.print("3");
  delay(1000);
  lcd.setCursor(7, 1);
  lcd.print("2");
  delay(1000);
  lcd.setCursor(7, 1);
  lcd.print("1");
  delay(1000);
}

//void showMonitor(String s) {
//
//}

void ModeClassic::gameOverScreen() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Game Over!");

  if (gm->score < 10) lcd.setCursor(2, 1);
  else lcd.setCursor(1, 1);
  lcd.print("Stg ");
  lcd.print(gm->stage + 1);

  lcd.print(", ");
  if (gm->score <= 1 || (gm->score >= 10 && gm->score < 100)) lcd.print(" "); // print extra space for alignment
  lcd.print(gm->score);
  lcd.print(" pt");
  if (gm->score > 1) lcd.print("s");
}

void ModeClassic::frenzyScreen() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Frenzy!");
  lcd.setCursor(1, 1);
  lcd.print("!Sudden Death!");
  delay(2000);
  updateMonitor();
}
