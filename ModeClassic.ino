#include "Game.h"

/**
 * Performs the logic check when a button is clicked.
 * First log the state of all buttons, then check if a button is triggered.
 * If so and the LED next to the button (bundled in a Block structure) is lit up green,
 * then:
 * 1. Add score
 * 2. Randomize target (and turn off previous target)
 * otherwise:
 * 1. Deduct score
 * 2. If current game state is Frenzy, changes state to game over upon wrong click (Insta-death).
 * Finally:
 * 1. Update the monitor to reflect new score
 * 2. Updates leds (showLed()) regardless
 */
void ModeClassic::clickLogic() {
  for (byte i = 0; i < NUM_BLOCKS; i++) {
    Block *b = blocks[i];

    b->storeInput();

    if (b->isTriggered()) {
      if (b->equals(CRGB::Green)) {
        // On CORRECT button click, change target; add 1 score; turn off current light
        score += this->scoreMultiplier;
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

   Implementation:
   This only runs under two conditions:
   1. Not game over
   2. Time's up for current stage.
   When these two conditions are satisfied, check whether player has reached the stage requirement score.
   If yes,
   1. Advance to next stage and reflect new requirements (updateStage()).
   2. Updates screen to reflect these changes.
   AND IF the new stage is the last stage,
   i. Change state to Frenzy
   ii. Plays the Frenzy intro screen
   Lastly,
   3. Changes beginMillis to millis(). (beginMillis is the "beginning time of the current stage")

   Otherwise (score not above requirement),
   1. Change state to Game Over
   2. Shows the game over screen.
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

      beginMillis = millis(); // Note: VERY IMPORTANT
    } else {
      this->state = ModeClassic::State::GameOver;
      // Play Game Over Screen
      this->gameOverScreen();
    }
  }
}

// ----------- Debut ------------
/**
 * Identical to ModeClassic's updateState() function but with Frenzy checek stripped out.
 */
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
