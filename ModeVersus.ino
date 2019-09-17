void ModeVersus::randomizeTarget(CRGB tColour, int player) {
  Player *p;
  byte *previousTarget;
  byte target;
  
  if (player == 1) {
    p = this->p1;
    previousTarget = &this->pTarget;
  } else if (player == 2) {
    p = this->p2;
    previousTarget = &this->pTarget2;
  }
//  else throw "Player must be either 1 or 2!"; // Need to add -fexceptions in command line to enable exceptions
  
  do {
    target = byte(random(0, NUM_BLOCKS/2));
  } while (target == pTarget);

  p->b[target]->setColour(tColour);

  previousTarget = &target;
}

void ModeVersus::clickLogic() {
  this->p1->clickLogic();
  this->p2->clickLogic();
}

void ModeVersus::updateState() {
  
}
