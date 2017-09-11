#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include "user_interface.h"
#include "wishock.h"
#include "state.h"

void setup() {
  stateSetup();
}

void loop() {
  stateUpdate();
  stateMachine();
}
