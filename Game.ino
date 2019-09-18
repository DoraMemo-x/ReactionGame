#include "Game.h"
#include "LED.h"
#include "InputHandler.h"

// ---------- Constants ----------------


// Functional Variables  (Variables that has a lower level usage. Not intuitive)
static unsigned long beginMillis = -1;

// Game Variables (Variables that are native to game logic)
Game *game; // Game base object (pointer) for polymorphism

/**
   Determine game mode
*/
void setupGame() {
  randomSeed(analogRead(A1));
  beginMillis = millis();

  Mode m = static_cast<Mode>(modeInput);

  delete game;

  switch (m) {
    //    default:
    case Classic: {
        ModeClassic *mc = new ModeClassic();
        game = mc;
        game->score = 100;

        game->setupGame();
        break;
      }

    ////////////////////////////

    case Debut: {
        ModeDebut *md = new ModeDebut();
        game = md;
        game->score = 90;

        game->setupGame();
        break;
      }

    ////////////////////////////

    case Versus: {
        ModeVersus *mv = new ModeVersus();
        game = mv;

//        game->setupGame();
        break;
      }

    ////////////////////////////

    case Colours:

      break;
  }

  //  game->setState(State::Init);
}

/**
   Choose a new random target that does not equal to the previous target,
   in the range of 0 ~ NUM_BLOCKS.
   Sets the colour of the target block's led to specified tColour
   @param tColour the colour of the target block's led
*/
void Game::randomizeTarget(CRGB tColour) {
  byte target;
  do {
    target = byte(random(0, NUM_BLOCKS));
  } while (target == this->pTarget);

  blocks[target]->setColour(tColour);

  pTarget = target;
}

/////////////////////////////



void setBeginMillis() {
  beginMillis = millis();
}

Mode indexToMode(int index) {
  return static_cast<Mode>(index);
}

int getSecondsRemaining() {
  return int((game->stageMs - (millis() - beginMillis)) / 1000);
}
