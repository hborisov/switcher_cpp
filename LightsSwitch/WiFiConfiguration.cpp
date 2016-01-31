#include "Arduino.h"
#include "EEPROM.h"
#include "WiFiConfiguration.h"

WiFiConfiguration::WiFiConfiguration() {
}

void WiFiConfiguration::load() {
  EEPROM.begin(512);
  byte isConfigured = EEPROM.read(IS_CONFIGURED_BYTE_ADDRESS);
  if (isConfigured != 0 && isConfigured != 1) {
    isConfigured = 0;
    return;
  }
  byte ssidSize = EEPROM.read(SSID_SIZE_ADDRESS);
  char _ssid[ssidSize];
    DEBUG_PRINTLN("SSID size: " + ssidSize);
    
  for (int i=0; i<ssidSize; i++) {
    char currentSSIDByte = EEPROM.read(SSID_START_ADDRESS + i);
    _ssid[i] = currentSSIDByte;
    DEBUG_PRINT(currentSSIDByte + ", ");
  }
  DEBUG_PRINTLN("");
  _ssid[ssidSize] = '\0';
  ssid = String(_ssid);

  int passwordSize = EEPROM.read(2 + ssidSize);
  char _password[passwordSize];
  DEBUG_PRINT("Password size: " + passwordSize);
  for (int i=0; i<passwordSize; i++) {
    char currentPasswordByte = EEPROM.read(3+ssidSize + i);
    _password[i] = currentPasswordByte;
    DEBUG_PRINT(currentPasswordByte + ", ");
  }
  DEBUG_PRINTLN("");
  _password[passwordSize] = '\0';
  password = String(_password);

  DEBUG_PRINTLN(ssid);
  DEBUG_PRINTLN(password);
  DEBUG_PRINTLN(_ssid);
  DEBUG_PRINTLN(_password);
  delay(500);
}

void WiFiConfiguration::save(void) {
  EEPROM.begin(3 + ssid.length() + password.length());
  EEPROM.write(IS_CONFIGURED_BYTE_ADDRESS, true);
  EEPROM.write(SSID_SIZE_ADDRESS, ssid.length());
  for (int i=0; i<ssid.length(); i++) {
    EEPROM.write(i+2, ssid.charAt(i));
  }

  EEPROM.write(2+ssid.length(), password.length());
  for (int i=0; i<password.length(); i++) {
    EEPROM.write(i+3+ssid.length(), password.charAt(i));
  }
  EEPROM.commit();
  
  delay(500);
}

String WiFiConfiguration::getSSID() {
  return ssid;
}

void WiFiConfiguration::setSSID(String _ssid) {
  ssid = _ssid;
}

String WiFiConfiguration::getPassword() {
  return password;
}

void WiFiConfiguration::setPassword(String _password) {
  password = _password;
}

