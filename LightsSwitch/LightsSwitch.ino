#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

const char* SWITCH_ID = "000015";

const char* APssid = "LightSwitch";
bool isConfigured = false;
char ssid[64];// = "borisov_2Ghz";
char password[64];// = "abcd1234";

int SWITCH_STATE = HIGH;

ESP8266WebServer server(80);

const int led = BUILTIN_LED;

void switchKey(int state) {
  digitalWrite(0, state);
  SWITCH_STATE = state;
  String message = "Switch " + String(SWITCH_ID) + " is now in state: " + String(SWITCH_STATE);
  server.send(200, "text/plain", message);
  Serial.println(message);
}

void switchOn() {
  switchKey(LOW);
  digitalWrite(BUILTIN_LED, LOW);
}

void switchOff() {
  switchKey(HIGH);
  digitalWrite(BUILTIN_LED, HIGH);
}

void getState() {
  String message = "{ \"id\": \"" + String(SWITCH_ID) + "\", \"state\": " + String(SWITCH_STATE) + ", \"ip\": \"" + String(WiFi.localIP()[0]) +"."+ String(WiFi.localIP()[1]) +"."+ String(WiFi.localIP()[2]) +"."+ String(WiFi.localIP()[3])+ "\" }";
  server.send(200, "text/plain", message);
}

void handleRoot() {
  server.send(200, "text/plain", "This is switch: " + String(SWITCH_ID) + "\n" +String(WiFi.localIP()[0])+String(WiFi.localIP()[1])+String(WiFi.localIP()[2])+"."+String(WiFi.localIP()[3])+String(WiFi.localIP()[4])+String(WiFi.localIP()[5])+"."+String(WiFi.localIP()[6])+String(WiFi.localIP()[7])+String(WiFi.localIP()[8])+"."+String(WiFi.localIP()[9])+String(WiFi.localIP()[10])+String(WiFi.localIP()[11]));
}

void loadConfig() {
  EEPROM.begin(512);
  isConfigured = EEPROM.read(0);
  int _ssidSize = EEPROM.read(1);
  char _ssid[_ssidSize];
  Serial.println(_ssidSize);
  Serial.println(", ");
  for (int i=0; i<_ssidSize; i++) {
    char tempByte = EEPROM.read(i+2);
    _ssid[i] = tempByte;
    Serial.print(tempByte);
    Serial.print(", ");
  }
  Serial.println("");
  _ssid[_ssidSize] = '\0';
  strncpy(ssid, _ssid, _ssidSize);
  //ssid = _ssid;

  int _passwordSize = EEPROM.read(_ssidSize+2);
  char _password[_passwordSize];
  Serial.println(_passwordSize);
  Serial.println(", ");
  for (int i=0; i<_passwordSize; i++) {
    char tempByte = EEPROM.read(i+3+_ssidSize);
    _password[i] = tempByte;
    Serial.print(tempByte);
    Serial.print(", ");
  }
  Serial.println("");
  _password[_passwordSize] = '\0';
  strncpy(password, _password, _passwordSize);
  //password = _password;

  Serial.println(_ssid);
  Serial.println(_password);
  delay(500);
}

void handleConfig() {
 if (server.hasArg("ssid") && server.hasArg("password")) {
    int _ssidSize = server.arg("ssid").length()+1;//sizeof(server.arg("ssid"));
    char _ssid[_ssidSize];
    server.arg("ssid").toCharArray(_ssid, sizeof(_ssid));
    //ssid = _ssid;
    strncpy(ssid, _ssid, _ssidSize);
    
    Serial.println(ssid);
    Serial.println(_ssid);
    Serial.println(_ssidSize);
    Serial.println(sizeof(_ssid));
    Serial.println(strlen(_ssid));

    int _passwordSize = server.arg("password").length()+1; //sizeof(server.arg("password"));
    char _password[_passwordSize];
    server.arg("password").toCharArray(_password, sizeof(_password));
    //password = _password;
    strncpy(password, _password, _passwordSize);
    
    Serial.println(password);
    Serial.println(_password);
    Serial.println(_passwordSize);
    Serial.println(sizeof(_password));
    Serial.println(strlen(_password));


    EEPROM.begin(_ssidSize+_passwordSize+3);
    EEPROM.write(0, isConfigured);
    EEPROM.write(1, sizeof(_ssid));
    for (int i=0; i<sizeof(_ssid); i++) {
      EEPROM.write(i+2, _ssid[i]);
    }

    EEPROM.write(sizeof(_ssid)+2, sizeof(_password));
    for (int i=0; i<sizeof(_password); i++) {
      EEPROM.write(i+3+sizeof(_ssid), _password[i]);
    }
    EEPROM.commit();
    
    delay(500);
    ESP.restart();
    server.send(200, "text/html", "Configured!");  
 } else {
    server.send(200, "text/html", "<html><head><title>Light Switch Config</title></head><body><form action=\"\"><input type=\"text\" name=\"ssid\"/><input type=\"password\" name=\"password\"/><input type=\"submit\"/><input type=\"reset\"/></form></body></html>");  
 }
}

void handleNotFound(){
  server.send(200, "text/plain", String(WiFi.localIP()[0]) +"."+ String(WiFi.localIP()[1]) +"."+ String(WiFi.localIP()[2]) +"."+ String(WiFi.localIP()[3]));
}

int connectToWiFi() {
    //WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting WiFi");
    Serial.println("SSID: ");
    Serial.println(ssid);
    Serial.println(String(ssid));
    Serial.println("Password: ");
    Serial.println(password);
    Serial.println(String(password));

    int i=0;
    while (WiFi.status() != WL_CONNECTED) {
      if (i == 30) {
        return -1;
      }
      
      delay(500);
      Serial.print(".");
      i++;
    }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(SWITCH_ID)) {
      Serial.println("MDNS responder started");
    }

    return 0;
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);

  Serial.println("Loading Config...");
  loadConfig();
  Serial.println("Loaded.");
  //if (isConfigured) {
    if (connectToWiFi() == -1) {
      WiFi.softAP(APssid);
      IPAddress myIP = WiFi.softAPIP();
      Serial.println("IP Address:");
      Serial.println(myIP);
      isConfigured = false;
    }
    //vSAP_Auth("68806692","e7rtP5ub");
  //} else {
    
  //}
  
  server.on("/on", switchOn);
  server.on("/off", switchOff);
  server.on("/state", getState);
  server.on("/config", handleConfig);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  Serial.println(ESP.getFreeSketchSpace());
}

void loop(void){
  //Serial.println("WIFI status:");
  //Serial.println(WiFi.status());
  if (isConfigured && WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
    //   vSAP_Auth("68806692","e7rtP5ub");

    server.on("/on", switchOn);
    server.on("/off", switchOff);
    server.on("/state", getState);
    server.on("/config", handleConfig);
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
  }

  
  int buttonState = digitalRead(3);
  if (buttonState == LOW) {
    do {
      buttonState = digitalRead(3);
      delay(15);
    } while (buttonState == LOW);

    if (SWITCH_STATE == HIGH) {
      SWITCH_STATE = LOW;
      switchOn();
    } else if (SWITCH_STATE == LOW) {
      SWITCH_STATE = HIGH;
      switchOff();
    }
  }

  server.handleClient();
}
