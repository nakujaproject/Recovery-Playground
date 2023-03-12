#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "defs.h"

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());// Function not known

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
//  client.subscribe(OUtopic);
//  client.publish(inTopic, "We are on ma nigga");

  Serial.print("Message sent [");
  Serial.print(topic);
  Serial.print("] ");
  // prints: Message arrived [topic]: below code prints the message character by character.
  for (int i = 0; i < length; i++) {
    msg[i] = (char)message[i];
    Serial.print(msg[i]);
  }
    Serial.println();//print an empty line
}
 void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
      Serial.print("Attempting MQTT connection...");
      // Create a random client ID
      String clientId = "GROUND-MCU";
      clientId += String(random(0xffff), HEX);
      // Attempt to connect
      if (client.connect(clientId.c_str())) {
        Serial.println("connected");
        // Once connected, publish an announcement...
//        client.subscribe(inTopic);
//        client.publish(inTopic, "Start");
        // ... and resubscribe
        client.subscribe("Control/Ejection");
      } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5);
      }
    }
  }
void setup() {
  // put your setup code here, to run once Serial.begin(9600);
  //  input_password.reserve(4); // maximum input characters is 33, change if needed
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);
  Serial.print("\n\nStarting program");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
  void loop() {
    // put your main code here, to run repeatedly:
//    client.subscribe(outTopic);
   //client.publish(outTopic, "We are gang ma nigga");
    if (!client.connected())  // Reconnect if connection is lost
    {
      reconnect();
    }
    client.loop();
//     manualOverride();
  buttonState=digitalRead(button);
 if(buttonState== LOW){
    digitalWrite(led, HIGH);
    client.publish(inTopic, "1");
    delay(500);
  }
  else if(buttonState==HIGH){
    digitalWrite(led,LOW);
    client.publish(inTopic, "0");
    delay(500);
  }
  }