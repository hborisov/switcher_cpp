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

