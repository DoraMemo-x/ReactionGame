#include "Game.h"

void ModeClassic::clickLogic() {
  for (byte i = 0; i < NUM_BLOCKS; i++) {
    Block *b = blocks[i];

    b->storeInput();

    if (b->isTriggered()) {
      if (b->equals(CRGB::Green)) {
        // On CORRECT button click, change target; add 1 score; turn off current light
        score += this->scoreMultiplier;
        b->setColour(CRGB::Black);
        this->randomizeTarget(CRGB::Green);

        delay(20); // delay a bit to avoid bouncing
      } else {
        // On WRONG click, remove score, incur minor delay
        if (state == ModeClassic::State::Frenzy) {
          this->state = ModeClassic::State::GameOver;
          // Play Game Over Screen
          this->gameOverScreen();
          return;
        }

        score = max(score - this->scorePenalty, 0);
        delay(25);
      }

      updateScoreMonitor(score, stageReq);
      showLed();
    }
  }
}

/**
   Determines game state (Win/Loss, Advance to next stage, etc.)
   and reflect the suitable changes.
*/
void ModeClassic::updateState() {
  // Time's up
  if (this->state != ModeClassic::State::GameOver && millis() - beginMillis > this->stageMs) {
    // Advance_stage or Game Over
    if (this->score >= this->stageReq) {
      this->stage++;
      this->updateStage(this->stage, CLASSIC_STAGE_MS, CLASSIC_STAGE_REQ, CLASSIC_SCORE_MULTIPLIER, CLASSIC_SCORE_PENALTY);
      
      this->ongoingScreen();

      // Frenzy
      if (this->stage == sizeof(CLASSIC_STAGE_REQ) / sizeof(byte) - 1) {
        this->state = ModeClassic::State::Frenzy;
        // Play Frenzy monitor
        this->frenzyScreen();
      }

      beginMillis = millis();
    } else {
      this->state = ModeClassic::State::GameOver;
      // Play Game Over Screen
      this->gameOverScreen();
    }
  }
}

// ----------- Debut ------------
void ModeDebut::updateState() {
  // Time's up
  if (this->state != ModeDebut::State::GameOver && millis() - beginMillis > this->stageMs) {
    // Advance_stage or Game Over
    if (this->score >= this->stageReq) {
      this->stage++;
      this->updateStage(this->stage, DEBUT_STAGE_MS, DEBUT_STAGE_REQ);
      
      this->ongoingScreen();

      beginMillis = millis();
    } else {
      this->state = ModeDebut::State::GameOver;
      // Play Game Over Screen
      this->gameOverScreen();
    }
  }
}
