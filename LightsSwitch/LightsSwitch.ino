#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* SWITCH_ID = "000009";

const char* ssid = "borisov_2Ghz";
const char* password = "abcd1234";

int SWITCH_STATE = LOW;

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
  switchKey(HIGH);
  digitalWrite(BUILTIN_LED, LOW);
}

void switchOff() {
  switchKey(LOW);
  digitalWrite(BUILTIN_LED, HIGH);
}

void getStateUserFriendly() {
  String message = "Switch " + String(SWITCH_ID) + " is now in state: " + String(SWITCH_STATE);
  server.send(200, "text/plain", message);
}

void getState() {
  String message = "{ \"id\": \"" + String(SWITCH_ID) + "\", \"state\": " + String(SWITCH_STATE) + " }";
  server.send(200, "text/plain", message);
}

void handleRoot() {
  server.send(200, "text/plain", "This is switch: " + String(SWITCH_ID));
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT);
  
  Serial.begin(115200,SERIAL_8N1,SERIAL_TX_ONLY); 
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

  server.on("/on", switchOff);
  server.on("/off", switchOn);
  server.on("/state", getState);
  server.on("/status", getStateUserFriendly);
  /*server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });*/

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
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
