/**
* Copyright 2016 Hristo Borisov
* 
* Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file 
* except in compliance with the License. You may obtain a copy of the License at
* 
* http://www.apache.org/licenses/LICENSE-2.0
* 
* Unless required by applicable law or agreed to in writing, software distributed under the License 
* is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
* either express or implied. See the License for the specific language governing 
* permissions and limitations under the License.
* 
*/

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

