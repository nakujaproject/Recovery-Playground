#include <Wire.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>


#define TX 17
#define RX 16
static const uint32_t GPSbaud = 9600;


HardwareSerial hard(2);
TinyGPSPlus gps;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  hard.begin(GPSbaud, SERIAL_8N1, RX, TX);
  Serial.println();
}


void loop() {
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

      //Number pf sattelites in use
      Serial.print("Number of satellites = ");
      Serial.println(gps.satellites.value());
      
      //Value of latitude in degrees
      Serial.print("Latitude = ");
      Serial.print(gps.location.lat(), 6);
      
      // Value of longitude in degrees
      Serial.print(" Longitude = ");
      Serial.println(gps.location.lng(), 6);

      // time
      Serial.print(" Time = ");
      Serial.println(gps.time.value());
      
      
      //Velocity
      Serial.print("Speed in m/s: ");
      Serial.println(gps.speed.mps());
  }
  Serial.print('.');
}