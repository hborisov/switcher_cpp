#include "Arduino.h"
#include "WiFiConfiguration.h"

InvertedSwitch::InvertedSwitch(int espPin) {
  sw = new Switch(espPin);
}

void InvertedSwitch::setOn() {
  sw->setOff();
}

void InvertedSwitch::setOff() {
  sw->setOn();
}

void InvertedSwitch::toggle() {
  sw->toggle();
}

int InvertedSwitch::getState() {
  if (sw->getState() == ON) {
    return OFF;
  }
  
  return ON;
}

