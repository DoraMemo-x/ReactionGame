#ifndef GAME_H
#define GAME_H

// ---------- Includes -------------



// ---------- Constants ----------------

static const unsigned int CLASSIC_STAGE_MS[] = {16000, 23000, 30000, 5000};
static const byte CLASSIC_STAGE_REQ[] = {22, 40 + CLASSIC_STAGE_REQ[0], 42 + CLASSIC_STAGE_REQ[1], 255}; // Accumulative score
static const byte CLASSIC_SCORE_MULTIPLIER[] = {1, 1, 1, 2};
static const byte CLASSIC_SCORE_PENALTY[] = {1, 1, 2, 0};

static const unsigned int DEBUT_STAGE_MS[] = {12500, 18000, 22000};
static const byte DEBUT_STAGE_REQ[] = {13, 25 + DEBUT_STAGE_REQ[0], 255}; // Accumulative score

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

// ------------------ Classes --------------------

class GameMode {
  public:
//    Mode mode = RETURN;
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

    // Function declarations
    virtual void clickLogic() = 0;
    virtual void updateState();
    virtual void updateStage(byte stage, unsigned int ms[], byte req[], byte multi[], byte penalty[]) {
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

    //    virtual void setupScreen() = 0;
    //    virtual void runningScreen() = 0;
    virtual void gameOverScreen() = 0;

  protected:
    State state; // NOTE: Necessary to write this in inherited classes, unless the State enum didn't get overriden
};

class ModeClassic : public GameMode {
  public:
    Mode mode = Classic;

    // Constructors
    ModeClassic() {
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

    void clickLogic();
    void updateState();
    boolean isGameOver() {
      return state == GameOver;
    }

    void frenzyScreen();
    void gameOverScreen();

    Mode getMode() {
      return Classic;
    }


  protected:
    State state;
};

class ModeDebut : public ModeClassic {
  public:
    Mode mode = Debut;
  
    // Enums
    enum State {
      Init,
      Ongoing,
      GameOver
    };

    void updateStage(byte stage, unsigned int ms[], byte req[]) {
      stageMs = ms[stage];
      stageReq = req[stage];
    }

    boolean isGameOver() {
      return state == GameOver;
    }

    // Constructors
    ModeDebut() {
      stage = 0;
      score = 0;
      scoreMultiplier = 1;
      scorePenalty = 1;
      updateStage(stage, DEBUT_STAGE_MS, DEBUT_STAGE_REQ);
    }

    void updateState();

    Mode getMode() {
      return Debut;
    }

  protected:
    State state;
};

// --------------- Extern Variables ---------------

extern GameMode *gm;
extern int score;

// -------------------- Function declarations -----------------

void setupGame();

void randomizeTarget(CRGB tColour);
Mode indexToMode(int index);
int getSecondsRemaining();

#endif
