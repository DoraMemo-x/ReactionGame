#ifndef GAME_H
#define GAME_H

// ---------- Includes -------------

//#include <FastLED.h>
#include "InputHandler.h"
#include "Player.h"
#include "LCDMonitor.h"

// ---------- Constants ----------------

static const unsigned int CLASSIC_STAGE_MS[] = {16000, 23000, 30000, 5000};
static const byte CLASSIC_STAGE_REQ[] = {22, 40 + CLASSIC_STAGE_REQ[0], 42 + CLASSIC_STAGE_REQ[1], 255}; // Accumulative score
static const byte CLASSIC_SCORE_MULTIPLIER[] = {1, 1, 1, 2};
static const byte CLASSIC_SCORE_PENALTY[] = {1, 1, 2, 0};

static const unsigned int DEBUT_STAGE_MS[] = {12500, 18000, 21500};
static const byte DEBUT_STAGE_REQ[] = {13, 25 + DEBUT_STAGE_REQ[0], 255}; // Accumulative score

static const unsigned int VERSUS_STAGE_MS[] = {20000, 20000, 15000};
static const byte VERSUS_STAGE_REQ[] = {15, 21, 27}; // Not accumulative.

// ----------- Enums --------------

enum Mode {
  Classic,
  Debut,
  Versus,
  Colours,
  RETURN
};

//enum ScoreSystem {
//  Click, Time
//};

// -------------------- Function declarations -----------------

void determineGameMode();

void setBeginMillis();
Mode indexToMode(int index);
int getSecondsRemaining();

// ------------------ Classes --------------------

class Game {
  public:
    byte score = 0;
    byte stage = 0;
    unsigned int stageMs;
    byte stageReq;
    byte scoreMultiplier;
    byte scorePenalty;

    // Enums
    enum State {
      Init,
      Ongoing,
      GameOver
    };

    // Constructors
    Game() {
      state = Init;
    }

    // Function declarations
    virtual void setupGame() {
      randomizeTarget(CRGB::Green);
      state = Init;
    }

    virtual void randomizeTarget(CRGB tColour);
    virtual void clickLogic() = 0;
    virtual void updateState();
    virtual void updateStage(byte stage, const unsigned int ms[], const byte req[], const byte multi[], const byte penalty[]) {
      stageMs = ms[stage];
      stageReq = req[stage];
      scoreMultiplier = multi[stage];
      scorePenalty = penalty[stage];
    }
    void setState(State newState) {
      state = newState;
    }
    State getState() {
      return state;
    }
    virtual Mode getMode() = 0;
    virtual boolean isGameOver() = 0;

    virtual void updateScreenTimeRemaining() {
      if (millis() - secTimer > 1000) {
        secTimer = millis();
        updateTime(getSecondsRemaining());
      }
    }
    virtual void ongoingScreen() = 0;
    virtual void gameOverScreen() = 0;

  protected:
    State state; // NOTE: Necessary to write this in inherited classes, unless the State enum didn't get overriden
    byte pTarget = -1;
    unsigned long secTimer = 0;
};



class ModeClassic : public Game {
  public:
    // Constructors
    ModeClassic() {
      state = Init;
      stage = 0;
      score = 0;
      updateStage(stage, CLASSIC_STAGE_MS, CLASSIC_STAGE_REQ, CLASSIC_SCORE_MULTIPLIER, CLASSIC_SCORE_PENALTY);
    }

    // Enums
    enum State {
      Init,
      Ongoing,
      Frenzy,
      GameOver
    };

    void clickLogic() override;
    void updateState() override;
    boolean isGameOver() override {
      return state == GameOver;
    }

    void ongoingScreen() override;
    void frenzyScreen();
    void gameOverScreen() override;

    Mode getMode() override {
      return Classic;
    }


  protected:
    State state;
};



class ModeDebut : public ModeClassic {
  public:
    // Enums
    enum State {
      Init,
      Ongoing,
      GameOver
    };

    void updateStage(byte stage, const unsigned int ms[], const byte req[]) {
      stageMs = ms[stage];
      stageReq = req[stage];
    }

    boolean isGameOver() override {
      return state == GameOver;
    }

    // Constructors
    ModeDebut() {
      state = Init;
      stage = 0;
      score = 0;
      scoreMultiplier = 1;
      scorePenalty = 1;
      updateStage(stage, DEBUT_STAGE_MS, DEBUT_STAGE_REQ);
    }

    void updateState() override;

    Mode getMode() override {
      return Debut;
    }

  protected:
    State state;
};



class ModeVersus : public Game {
  public:
    // "stage" is used as number of rounds
    // "score" is unused
    // "stageReq" is ONLY used for first round, as a way to pass a common requirement for both players
    // "scoreMultiplier" is unused
    // "scorePenalty" is unused
    Player *p1;
    Player *p2;

    // Constructors
    ModeVersus() {
      setupGame();
    }

    // Enums
    enum State {
      Init,
      Ongoing,
      GameOver
    };

    void setupPlayers();
    void setupGame() override {
      stage = 0;
      stageMs = VERSUS_STAGE_MS[0];
      stageReq = VERSUS_STAGE_REQ[0];
      setupPlayers();
      p1->randomizeTarget();
      p2->randomizeTarget();
      state = Init;
    }

    //    void randomizeTarget(int player);
    void clickLogic() override;
    void updateState() override;
    boolean isGameOver() override {
      return state == GameOver;
    }

    void updateScreenTimeRemaining() override {
      if (millis() - secTimer > 1000) {
        secTimer = millis();
        updateTime(5, getSecondsRemaining());
      }
    }
    void scoreboard(int p1Score, int p2Score);
    void nextRoundScreen();
    void ongoingScreen() override;
    void gameOverScreen() override;

    Mode getMode() {
      return Versus;
    }

  protected:
    State state;
};

// --------------- Extern Variables ---------------

extern Game *game;
extern int score;

#endif
