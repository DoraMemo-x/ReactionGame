#include "Game.h"
#include "LED.h"
#include "BoardHandler.h"

// ---------- Constants ----------------


// Functional Variables  (Variables that has a lower level usage. Not intuitive)
static unsigned long beginMillis = -1;

// Game Variables (Variables that are native to game logic)
Game *game; // Game base object (pointer) for polymorphism

int seed = 0;

/**
 * Determines game mode.
 * This has to be run BEFORE setupMonitor().
*/
void determineGameMode() {
//  seed = analogRead(A0);
//seed++;
  randomSeed(86);
//  randomSeed(seed);
  beginMillis = millis();

  Mode m = static_cast<Mode>(modeInput);

  delete game;

  switch (m) {
    //    default:
    case Classic: {
        ModeClassic *mc = new ModeClassic();
        game = mc;
        game->score = 100;
        break;
      }

    ////////////////////////////

    case Debut: {
        ModeDebut *md = new ModeDebut();
        game = md;
        game->score = 90;
        break;
      }

    ////////////////////////////

//default:
    case Versus: {
        ModeVersus *mv = new ModeVersus();
        game = mv;
        break;
      }

    ////////////////////////////

//    case Colours:
//
//      break;
  }

  game->restartPeriodTimer();
}

/**
 * Choose a new random target that isn't the same as the previous target,
 * in the range of 0 ~ NUM_BLOCKS.
 * Sets the colour of the target block's led to specified tColour
 * @param tColour the colour of the target block's led
*/
void Game::randomizeTarget(CRGB tColour) {
  byte target;
  do {
    target = byte(random(0, NUM_BLOCKS));
  } while (target == pTarget);

  if (pTarget != -1) blocks[pTarget]->setColour(CRGB::Black);
  blocks[target]->setColour(tColour);

  pTarget = target;
}

/////////////////////////////



/**
 * 
 */
void setBeginMillis() {
  beginMillis = millis();
}

Mode indexToMode(int index) {
  return static_cast<Mode>(index);
}

int getSecondsRemaining() {
  return int((game->stageMs - (millis() - beginMillis)) / 1000);
}
