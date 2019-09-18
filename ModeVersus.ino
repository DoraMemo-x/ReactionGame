#include "Game.h"

void ModeVersus::clickLogic() {  
  boolean needUpdate = this->p1->clickLogic() || this->p2->clickLogic();
  if (needUpdate) {
    this->ongoingScreen();
  }
}

void ModeVersus::updateState() {
  byte p1Req = p1->scoreReq - p1->score;
  byte p2Req = p2->scoreReq - p2->score;
  byte winner = 0;

  // Round over check

  if (millis() - beginMillis >= this->stageMs) {
    if (p1->score > p2->score) winner = 1;
    else if (p2->score > p1->score) winner = 2;
    else winner = 3;
  }

  if (p1Req <= 0) {
    p1->wins++;
    winner = 1;
  }
  if (p2Req <= 0) {
    p2->wins++;
    winner = 2;
  }

  // Resetting round

  if (winner > 0) {
    this->stage++;

    // Determine & reflect game over
    if (this->stage == 3 || p1->wins == 2 || p2->wins == 2) {
      state = ModeVersus::State::GameOver;
      this->gameOverScreen();
      return;
    }

    // Set time
    this->stageMs = VERSUS_STAGE_MS[this->stage];

    // Randomize Targets
    this->randomizeTarget(1);
    this->randomizeTarget(2);

    // Adjust score requirement
    byte diff = abs(p1Req - p2Req);
    byte offset = 0;
    if (diff >= 5) offset = min(5, map(diff, 5, 10, 1, 5));

    this->p1->scoreReq = VERSUS_STAGE_REQ[this->stage];
    this->p2->scoreReq = VERSUS_STAGE_REQ[this->stage];

    if (winner == 1) this->p1 += offset;
    else if (winner == 2) this->p2 += offset;

    // Show scoreboard. Countdown & show next game
    this->scoreboard(p1->score, p2->score);
    this->nextRoundScreen();
    beginMillis = millis();

    // Reset score
    this->p1->score = 0;
    this->p2->score = 0;

    this->ongoingScreen();
  }
}
