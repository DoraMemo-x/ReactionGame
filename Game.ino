#include "Game.h"
#include "LED.h"
#include "InputHandler.h"

// ---------- Constants ----------------


// Functional Variables  (Variables that has a lower level usage. Not intuitive)
static long beginMillis = -1;
static byte pTarget = -1;

// Game Variables (Variables that are native to game logic)
GameMode *gm; // GameMode base object (pointer) for polymorphism

/**
   Determine game mode
*/
void setupGame() {
  randomSeed(analogRead(A1));
  beginMillis = millis();

  Mode m = Classic;
//  Mode m = static_cast<Mode>(modeInput);

  switch (m) {
    default:
    case Classic: {
        ModeClassic mc;
        gm = &mc;
        gm->score = 100;

        randomizeTarget(CRGB::Green);
        break;
      }

    ////////////////////////////

    case Debut: {
        ModeDebut md;
        gm = &md;
        gm->score = 90;

        randomizeTarget(CRGB::Green);
        break;
      }

    ////////////////////////////

    case Versus:

      break;

    ////////////////////////////

    case Colours:

      break;
  }

  //  gm->setState(State::Init);
}

void setBeginMillis() {
  beginMillis = millis();
}

void ModeClassic::clickLogic() {
  for (int i = 0; i < NUM_BLOCKS; i++) {
    Block *b = &blocks[i];

    b->storeInput();

    if (b->isTriggered()) {
      if (b->equals(CRGB::Green)) {
        // On CORRECT button click, change target; add 1 score; turn off current light
        score += this->scoreMultiplier;
        b->setColour(CRGB::Black);
        randomizeTarget(CRGB::Green);

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

      updateScore(score, stageReq);
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
      updateMonitor();

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

/**
   Choose a new random target that does not equal to the previous target,
   in the range of 0 ~ NUM_BLOCKS.
   Sets the colour of the target block's led to specified tColour
   @param tColour the colour of the target block's led
*/
void randomizeTarget(CRGB tColour) {
  byte target;
  do {
    target = byte(random(0, NUM_BLOCKS));
  } while (target == pTarget);

  blocks[target].setColour(tColour);

  pTarget = target;
}



void ModeDebut::updateState() {
  // Time's up
  if (this->state != ModeDebut::State::GameOver && millis() - beginMillis > this->stageMs) {
    // Advance_stage or Game Over
    if (this->score >= this->stageReq) {
      this->stage++;
      this->updateStage(this->stage, DEBUT_STAGE_MS, DEBUT_STAGE_REQ);
      updateMonitor();

      beginMillis = millis();
    } else {
      this->state = ModeDebut::State::GameOver;
      // Play Game Over Screen
      this->gameOverScreen();
    }
  }
}

/////////////////////////////

Mode indexToMode(int index) {
  return static_cast<Mode>(index);
}

int getSecondsRemaining() {
  return int((gm->stageMs - (millis() - beginMillis)) / 1000);
}
