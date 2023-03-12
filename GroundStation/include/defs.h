#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#define BAUD_RATE 115200

// #define ssid "iPIC-WIRELESS"
// #define password "987654321jica"
const char *inTopic = "Control/Ejection";
const char *outTopic = "Control/Ejection/Success";
// const char* mqtt_server = "";

#define ssid ""
#define password ""
const char* mqtt_server = "";

// #define DEBUG 1
// #if DEBUG == 1
// #define debug(x) Serial.print(x)
// #define debugln(x) Serial.println(x)
// #define debugf(x, y) Serial.printf(x, y)
// #else
// #define debug(x)
// #define debugln(x)
// #define debugf(x, y)
// #endif

#define led 18
#define button 4
int buttonState;




unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  50
char msg[MSG_BUFFER_SIZE];


#endif