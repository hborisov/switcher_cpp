#ifndef WiFiConfiguration_h
#define WiFiConfiguration_h

#include "Arduino.h"
#include "EEPROM.h" 

#define IS_CONFIGURED_BYTE_ADDRESS  0
#define SSID_SIZE_ADDRESS           1
#define SSID_START_ADDRESS          2

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


#endif
