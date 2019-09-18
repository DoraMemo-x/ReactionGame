#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LCDMonitor.h"
#include "Game.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setupMonitor() {
  lcd.init();
  lcd.backlight();

  switch (game->getMode()) {
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
      lcd.setCursor(5, 0);
      lcd.print("Versus");
      break;

    ////////////////////////////

//    case Colours:
//      lcd.setCursor(5, 0);
//      lcd.print("Colours");
//      break;
  }

  lcd.setCursor(6, 1);
  lcd.print("Mode");

  delay(1500);

  //  clearLine(0);
  //  lcd.setCursor(5, 0);
  //  lcd.print("Ready?");
  //  countdown();

  game->ongoingScreen();
}

void updateTime(byte timeRemain) {
  updateTime(9, timeRemain);
}

void updateTime(byte pos, byte timeRemain) {
  lcd.setCursor(pos, 0);
  lcd.print("        ");
  lcd.setCursor(pos, 0);
  lcd.print("(");
  lcd.print(timeRemain + 1);
  lcd.print(" s)");
}

void updateStageMonitor(byte stage, int timeRemain) {
  clearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("Stage: ");
  lcd.print(stage + 1);
  lcd.print(" (");
  lcd.print(timeRemain + 1);
  lcd.print(" s)");
}

void updateScoreMonitor(int score, int req) {
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

void countdown() {
  clearLine(1);
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

void ModeClassic::ongoingScreen() {
  updateStageMonitor(this->stage, getSecondsRemaining());
  updateScoreMonitor(this->score, this->stageReq);
}

void ModeClassic::gameOverScreen() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Game Over!");

  if (game->score < 10) lcd.setCursor(2, 1);
  else lcd.setCursor(1, 1);
  lcd.print("Stg ");
  lcd.print(game->stage + 1);

  lcd.print(", ");
  if (game->score <= 1 || (game->score >= 10 && game->score < 100)) lcd.print(" "); // print extra space for alignment
  lcd.print(game->score);
  lcd.print(" pt");
  if (game->score > 1) lcd.print("s");
}

void ModeClassic::frenzyScreen() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Frenzy!");
  lcd.setCursor(1, 1);
  lcd.print("!Sudden Death!");
  delay(2000);
  this->ongoingScreen();
}



void ModeVersus::scoreboard(int p1Score, int p2Score) {
  clearLine(0);
  clearLine(1);

  if (p1Score > p2Score) {
    lcd.setCursor(0, 0);
    lcd.print("Winner!");
  } else if (p2Score > p1Score) {
    lcd.setCursor(9, 0);
    lcd.print("Winner!");
  } else {
    lcd.setCursor(6, 0);
    lcd.print("Draw");
  }
  lcd.setCursor(0, 1);
  lcd.print(p1Score);
  if (p2Score < 10) lcd.setCursor(15, 1);
  else lcd.setCursor(14, 1);
  lcd.print(p2Score);
}

void ModeVersus::nextRoundScreen() {
  delay(3000);

  clearLine(0);
  lcd.setCursor(4, 0);
  lcd.print("Round ");
  lcd.print(this->stage + 1);
  countdown();
}

void ModeVersus::ongoingScreen() {
  clearLine(1);
  byte p1TargetScore = p1->scoreReq - p1->score;
  byte p2TargetScore = p2->scoreReq - p2->score;
  
  byte p1FullBlocks = floor(p1TargetScore / 4);
  byte p1Remain = p1TargetScore % 4;
  byte p2FullBlocks = floor(p2TargetScore / 4);
  byte p2Remain = p2TargetScore % 4;

  lcd.setCursor(0, 1);
  for (byte i = 0; i < p1FullBlocks; i++) {
    lcd.print("#");
  }
  if (p1Remain == 3) lcd.print("*");
  else if (p1Remain == 2) lcd.print("+");
  else if (p1Remain == 1) lcd.print("|");



  lcd.setCursor(16 - p2FullBlocks - 1, 1);
  if (p2Remain == 0) lcd.print(" ");

  if (p2Remain == 3) lcd.print("*");
  else if (p2Remain == 2) lcd.print("+");
  else if (p2Remain == 1) lcd.print("|");
  for (byte j = 0; j < p2FullBlocks; j++) {
    lcd.print("#");
  }
}

void ModeVersus::gameOverScreen() {
  this->scoreboard(this->p1->wins, this->p2->wins);

  lcd.setCursor(7, 1);
  lcd.print("VS");
}
