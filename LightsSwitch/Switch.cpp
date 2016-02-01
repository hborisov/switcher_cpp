#include "Arduino.h"
#include "WiFiConfiguration.h"

Switch::Switch(int pin) {
  espPin = pin;
  state = OFF;
}

Switch::Switch(int pin, int initialState) {
  espPin = pin;
  state = initialState;
}

void Switch::setState(int newState) {
  state = newState;
  digitalWrite(espPin, state);
}

int Switch::getState() {
  return state;
}

void Switch::setOn() {
  setState(ON);
}

void Switch::setOff() {
  setState(OFF);
}

void Switch::toggle() {
  if (getState() == ON) {
    setOff();
  } else if (getState() == OFF) {
    setOn();
  }
}

