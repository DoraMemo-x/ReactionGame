#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LCDMonitor.h"
#include "Game.h"

LiquidCrystal_I2C lcd(0x3F, 16, 2);

/**
   Shows the current gamemode on LCD monitor,
   followed by a short delay, then a countdown.
   This shows, for example, "   CLASSIC   ;     MODE     " on the screen (; means new line)
   NOTE: This has to be run AFTER determineGameMode().
*/
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

  // TODO: Uncomment this
  //  clearLine(0);
  //  lcd.setCursor(5, 0);
  //  lcd.print("Ready?");
  //  countdown();

  game->ongoingScreen();
}

/**
   Updates time remaining on screen (default position on the screen),
   in the form "(T s)" where T = timeRemain.
   @param timeRemain supplied time remaining parameter
*/
void updateTime(byte timeRemain) {
  updateTime(9, timeRemain);
}

/**
   Updates time remaining on screen at specified position pos on the screen,
   in the form "(T s)" where T = timeRemain.
   @param pos Specified position of where the string "(TT s)" should appear.
   @param timeRemain supplied time remaining parameter
*/
void updateTime(byte pos, byte timeRemain) {
  lcd.setCursor(pos, 0);
  lcd.print("        ");
  lcd.setCursor(pos, 0);
  lcd.print("(");
  lcd.print(timeRemain + 1);
  lcd.print(" s)");
}

/**
   Updates the stage and the current remaining time info on the screen,
   in the form "Stage: X (T s)" where X = stage and T = timeRemain.
   Currently used by ModeClassic and ModeDebut.
   (TODO: Change this to a class method)
   (TODO: Change this to only update the stage)
   @param stage Current stage
   @param timeRemain supplied time remaining parameter
*/
void updateStageMonitor(byte stage, int timeRemain) {
  clearLine(0);
  lcd.setCursor(0, 0);
  lcd.print("Stage: ");
  lcd.print(stage + 1);
  lcd.print(" (");
  lcd.print(timeRemain + 1);
  lcd.print(" s)");
}

/**
   Updates the score and the score requirement to pass
*/
void updateScoreMonitor(int score, int req) {
  clearLine(1);
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.print(" / ");
  lcd.print(req);
}

/**
   Clears one line of the screen.
   @param i line number. Can either be 0 or 1 on this 16x2 LCD monitor.
*/
static void clearLine(int i) {
  lcd.setCursor(0, i);
  lcd.print("                ");
}

/**
   Initiate a countdown timer on the bottom line of the 16x2 monitor.
   The countdown lasts for 3 seconds.
*/
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

/**
   TODO: Shows a running text on the screen
*/
//void showMonitor(String s) {
//
//}

/**
   Updates both score and stage listed on the screen.
   The text should look like this:
   "Stage: 1 (15s)"
   "Score: 0/20"
*/
void ModeClassic::ongoingScreen() {
  updateStageMonitor(this->stage, getSecondsRemaining());
  updateScoreMonitor(this->score, this->stageReq);
}

/**
   Shows game over stats on the screen.
   The text should look like this:
   "   Game Over!   "
   " Stg 1, 10 pts "
*/
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

/**
   Indicates the game state has advanced to Frenzy by showing this text:
   "Frenzy!"
   "!Sudden Death!",
   followed by a 2-second delay, then switches back to the stage and score (ongoing) screen.
*/
void ModeClassic::frenzyScreen() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Frenzy!");
  lcd.setCursor(1, 1);
  lcd.print("!Sudden Death!");
  delay(2000);
  this->ongoingScreen();
}



/**
   Shows the result of a 1v1 mode. Indicates the winner by comparing p1Score and p2Score.
   @p1Score Score on the left
   @p2Score Score on the right
*/
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

/**
   Shows this text before a round starts:
   "Round X" where X is the current stage from 1-3.
   This is followed by a countdown of 3 seconds.
*/
void ModeVersus::nextRoundScreen() {
  delay(3000);

  clearLine(0);
  lcd.setCursor(4, 0);
  lcd.print("Round ");
  lcd.print(this->stage + 1);
  countdown();
  lcd.clear();
}

/**
   Shows a "deduct style" of points on screen.
   For example, "###*  |####".
   Implementation:
   The required score is converted to base-4 with these characters:
   # = 4
 * * = 3
   + = 2
   | = 1.
   (Number of strokes on the symbol = its value)
   NOTE: This does not do overflow check.
*/
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

/**
   Shows the final result of the 1v1 game.
*/
void ModeVersus::gameOverScreen() {
  // Following line: A "hack" that forces the timer to not overwrite the Game Over text
  periodTimer = millis();

  scoreboard(p1->wins, p2->wins);

  lcd.setCursor(3, 1);
  lcd.print("Game Over!");
}
