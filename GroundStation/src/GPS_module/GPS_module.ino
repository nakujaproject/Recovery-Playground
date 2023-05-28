#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>


#define TX 16
#define RX 17
static const uint32_t GPSbaud = 9600;
const char* ssid = "";
const char* password = "";
const char* mqtt_serv = "192.168.100.7";


HardwareSerial hard(2);
TinyGPSPlus gps;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  hard.begin(GPSbaud, SERIAL_8N1, RX, TX);
  Serial.println();
  Serial.print("connecting to: ");
  Serial.println(ssid);
// Connecting to WiFi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected, IP: ");
  Serial.print(WiFi.localIP());
  client.setServer(mqtt_serv, 1883);
}


void loop() {
  // put your main code here, to run repeatedly:
  if (client.connected())
  {
    reconnect();
  }

  
  while (hard.available() > 0)
  {
    gps.encode(hard.read());
  }
    if (gps.location.isUpdated())
    {
      StaticJsonDocument<80> doc;
      char output[80];
      
      doc['l'] = (gps.location.rawLat().deg);
      doc['L'] = (gps.location.rawLng().deg);
      doc['V'] = (gps.speed.mps());
      doc['S'] = (gps.satellites.value());
      
      serializeJson(doc, output);

      client.publish("/sensor/GPS", output);
      //Number pf sattelites in use
      Serial.print("Number of satellites = ");
      Serial.println(gps.satellites.value());
      
      //Value of latitude in degrees
      Serial.print("Latitude = ");
      Serial.print(gps.location.lat(), 6);
      
      // Value of longitude in degrees
      Serial.print(" Longitude = ");
      Serial.println(gps.location.lng(), 6);
      
      //Velocity
      Serial.print("Speed in m/s: ");
      Serial.println(gps.speed.mps());
  }
  client.loop();
}

void reconnect(){
  while(!client.connected()){
    Serial.println("MQTT not connected ... trying to connect");
    String clientId = "ESP32Client-";
    clientId = String(random(0xffff), HEX);

    if(client.connect(clientId.c_str())){
      Serial.println("Connected!");
    }
    else{
      Serial.println("Failed code =");
      Serial.println(client.state());
      delay(5000);
    }
  }
}
