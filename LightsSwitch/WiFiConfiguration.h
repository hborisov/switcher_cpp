#ifndef WiFiConfiguration_h
#define WiFiConfiguration_h

#include "Arduino.h"
#include "EEPROM.h"
#include <Adafruit_NeoPixel.h>

#define IS_CONFIGURED_BYTE_ADDRESS  0
#define SSID_SIZE_ADDRESS           1
#define SSID_START_ADDRESS          2
static const int ON = 1;
static const int OFF = 0;

#define IS_DEBUG
#ifdef IS_DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif




class WiFiConfiguration {
   public:
    WiFiConfiguration();
    void load();
    void save();
    String getSSID();
    void setSSID(String _ssid);
    String getPassword();
    void setPassword(String _password);
    
   private:
    String  ssid;
    String  password;
};

class Switch {
  public:
    Switch(int pin);
    Switch(int pin, int initialState);
    int getState();
    void setOn();
    void setOff();
    void toggle();
  private:
    int state;
    int espPin;
    void setState(int newState);
};

class InvertedSwitch {
  public:
    InvertedSwitch(int espPbin);
    void setOn();
    void setOff();
    void toggle();
    int getState();
  private:
    Switch *sw;
};

class Display {
  public:
    Display(int numberOfPixels);
    void showLoading();
    void showOn();
    void showOff();
    void setFrame(int frame[][3]);
  private:
    Adafruit_NeoPixel screen;
    int numberOfPixels;
};

#endif
