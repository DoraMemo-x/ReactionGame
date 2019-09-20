#include "Game.h"

void ModeVersus::clickLogic() {  
  boolean p1Clicked = p1->clickLogic();
  boolean p2Clicked = p2->clickLogic();
  if (p1Clicked || p2Clicked) {
    this->ongoingScreen();
  }
}

/**
 * Read the comments. It's pretty straightforward
 */
void ModeVersus::updateState() {
  byte p1Score = p1->score;
  byte p2Score = p2->score;
  byte p1Req = p1->scoreReq - p1Score;
  byte p2Req = p2->scoreReq - p2Score;
  byte winner = 0;

  // Round over check

  if (millis() - beginMillis >= stageMs) {
    if (p1->score > p2->score) winner = 1;
    else if (p2->score > p1->score) winner = 2;
    else winner = 3;
  }

  if (p1Req <= 0 || winner == 1) {
    p1->wins++;
    winner = 1;
  }
  if (p2Req <= 0 || winner == 2) {
    p2->wins++;
    winner = 2;
  }

  // Resetting round

  if (winner > 0) {
    Serial.print("Winner = ");
    Serial.println(winner);
    Serial.print("Round Wins: ");
    Serial.print(p1->wins);
    Serial.println(p2->wins);
    Serial.print("Next stage: ");
    Serial.println(stage + 1);
    
    stage++;

    // Determine & reflect game over
    if (stage == 3 || p1->wins == 2 || p2->wins == 2) {
      state = ModeVersus::State::GameOver;
      gameOverScreen();
      return;
    }

    // Set time
    stageMs = VERSUS_STAGE_MS[stage];

    // Randomize Targets
    p1->randomizeTarget();
    p2->randomizeTarget();

    // Adjust score requirement
    byte diff = abs(p1Req - p2Req);
    byte offset = 0;
    if (diff >= 5) offset = min(ceil((diff - 5)*0.5), 4);

    p1->scoreReq = VERSUS_STAGE_REQ[stage];
    p2->scoreReq = VERSUS_STAGE_REQ[stage];

    if (winner == 2) p1 -= offset;
    else if (winner == 1) p2 -= offset;

    // Show scoreboard. Countdown & show next round
    scoreboard(p1Score, p2Score);
    nextRoundScreen();
    beginMillis = millis();

    // Reset score
    p1->score = 0;
    p2->score = 0;

    ongoingScreen();
  }
}
