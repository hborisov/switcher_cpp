#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

const char* SWITCH_ID = "000011";

const char* APssid = "LightSwitch";
bool isConfigured = false;
const char* ssid = "";//"borisov_2Ghz";
const char* password = "";//"abcd1234";
//const char* ssid = "InnoWi-Fi";
//const char* password = "innoweek";

int SWITCH_STATE = HIGH;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

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

void handleConfig() {
 if (server.hasArg("ssid") && server.hasArg("password")) {
    isConfigured = true;
    char _ssid[sizeof(server.arg("ssid"))+1];
    server.arg("ssid").toCharArray(_ssid, sizeof(_ssid));
    ssid = _ssid;
    Serial.println(ssid);
    char _password[sizeof(server.arg("password"))];
    server.arg("password").toCharArray(_password, sizeof(_password));
    password = _password;
    Serial.println(password);
 } else {
    server.send(200, "text/html", "<html><head><title>Light Switch Config</title></head><body><form action=\"\"><input type=\"text\" name=\"ssid\"/><input type=\"password\" name=\"password\"/><input type=\"submit\"/><input type=\"reset\"/></form></body></html>");  
 }
}

void handleNotFound(){
  server.send(200, "text/plain", String(WiFi.localIP()[0]) +"."+ String(WiFi.localIP()[1]) +"."+ String(WiFi.localIP()[2]) +"."+ String(WiFi.localIP()[3]));
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
    Serial.println("");
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin(SWITCH_ID)) {
      Serial.println("MDNS responder started");
    }
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY);
  if (isConfigured) {
    connectToWiFi();
  } else {
    WiFi.softAP(APssid);  
  }
  
  server.on("/on", switchOn);
  server.on("/off", switchOff);
  server.on("/state", getState);
  server.on("/config", handleConfig);
  server.onNotFound(handleNotFound);

  httpUpdater.setup(&server);
  server.begin();
  Serial.println("HTTP server started");
  Serial.println(ESP.getFreeSketchSpace());
}

void loop(void){
  if (isConfigured && WiFi.status() != WL_CONNECTED) {
    connectToWiFi();

    server.on("/on", switchOn);
    server.on("/off", switchOff);
    server.on("/state", getState);
    server.on("/config", handleConfig);
    server.onNotFound(handleNotFound);
  
    httpUpdater.setup(&server);
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
