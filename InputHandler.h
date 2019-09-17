#ifndef INPUT_H
#define INPUT_H

// ---------- Includes -------------

#include <FastLED.h>

// ---------- Constants ----------------

#define NUM_BLOCKS 6

// --------------- Classes -------------------
/**
   A Block is made up of the button itself and the corresponding LED.
*/
class Block {
  public:
    boolean btnState = false;
    boolean pBtnState = false;
    CRGB colour;
    
    // Constructor
    Block(int btn, int led);

    // Functions
    void storeInput();
    boolean isTriggered();
    boolean equals(CRGB colour);

    // Setters
    void setColour(CRGB colour);

    // Getters
    int getButtonPin();
  private:
    int buttonPin;
    int ledIndex;
};

// --------------- Extern Variables ---------------

extern Block *blocks[NUM_BLOCKS];
extern byte modeInput;

// ---------------- Function Prototypes ---------------

void setupInput();

void storeInputAll(Block *blocks[]); // NOTE: This doesn't work for some reason. Althought explicitly stated the parameter is Block*[], it gets converted to Block**.

#endif
