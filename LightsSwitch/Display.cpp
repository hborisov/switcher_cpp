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
#include <Adafruit_NeoPixel.h>

int ON_FRAME[][3] = { {0,0,0},  {0,40,0}, {0,40,0}, {0,0,0},
                      {0,40,0}, {0,0,0},  {0,0,0},  {0,40,0},
                      {0,40,0}, {0,0,0},  {0,0,0},  {0,40,0},
                      {0,0,0},  {0,40,0}, {0,40,0}, {0,0,0} };

int OFF_FRAME[][3] = { {40,0,0}, {0,0,0},  {0,0,0},  {40,0,0},
                       {0,0,0},  {40,0,0}, {40,0,0}, {0,0,0},
                       {0,0,0},  {40,0,0}, {40,0,0}, {0,0,0},
                       {40,0,0}, {0,0,0},  {0,0,0},  {40,0,0} };

int LOADING_FRAME[][3] = { {40,40,40}, {40,40,40}, {40,40,40}, {40,40,40},
                           {40,40,40}, {40,40,40}, {40,40,40}, {40,40,40},
                           {40,40,40}, {40,40,40}, {40,40,40}, {40,40,40},
                           {40,40,40}, {40,40,40}, {40,40,40}, {40,40,40} };


Display::Display(int numberOfPixels) {
  numberOfPixels = numberOfPixels;
  screen = Adafruit_NeoPixel(numberOfPixels, 2, NEO_GRB + NEO_KHZ800);
  screen.begin(); // This initializes the NeoPixel library.
}

void Display::setFrame(int frame[][3]) {
  for (int i=0; i<16; i++) {
    screen.setPixelColor(i, screen.Color(frame[i][0],frame[i][1],frame[i][2]));
  }
  delay(50);
  screen.show();
}

void Display::showLoading() {
  setFrame(LOADING_FRAME);
}

void Display::showOn() {
  setFrame(ON_FRAME);
}

void Display::showOff() {
  setFrame(OFF_FRAME);
}

