//This project seeks to collect data from two sensors: altimeter180 and gyroscope6050 through an ESP32 
//   and store that data to a flash memory.
// In this case we use the Windbond W25Q128FV flash memory which is a 128MB flash memory chip 
//  that supports SPI and QPI



//importing libraries for use in the project
#include <Arduino.h>
#include <Adafruit_altimeter085.h>
#include <Adafruit_gyroscope6050.h>
#include <Adafruit_Sensor.h>
#include <SPIMemory.h>

Adafruit_gyroscope6050 gyroscope;
Adafruit_altimeter085 altimeter;

SPIFlash flash(SS, &SPI);

void setup() {
  //setup altimeter180, gyroscope6050 to write data to esp32
  //setup sdcard and eeprom to read data from esp32
  Serial.begin(115200);

  //setup flash memory
  flash.begin();
  flash.eraseChip();

  Serial.print(F("Flash size is: "));
  Serial.print((long)(flash.getCapacity()/1000));
  Serial.println(F("kb"));

  //Check for status of sensors
  if(!altimeter.begin()){
    Serial.println("altimeter180 not found");
    while (1) {
      delay(10);
    }
  }
  if (!gyroscope.begin()){
    Serial.println("gyroscope6050 not found");
    while (1){
      delay(10);
    }
  }

  Serial.println("Adafruit gyroscope6050 found!");
  Serial.println("Adafruit altimeter180 found!");

  //setup the gyroscope6050
  gyroscope.setAccelerometerRange(gyroscope6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  Serial.println(gyroscope.getAccelerometerRange());
 
  gyroscope.setGyroRange(gyroscope6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  Serial.println(gyroscope.getGyroRange());
 
  gyroscope.setFilterBandwidth(gyroscope6050_BAND_5_HZ);
  Serial.println("Filter bandwidth set to: ");
  Serial.println(gyroscope.getFilterBandwidth());

  delay(100);  
}

void loop() {
  //read gyroscope values
  sensors_event_t acc, rot, temp;
  gyroscope.getEvent(&acc, &rot , &temp);

  float x_acc = acc.acceleration.x;
  float y_acc = acc.acceleration.y;
  float z_acc = acc.acceleration.z;

  float x_gyro = rot.gyro.x;
  float y_gyro = rot.gyro.y;
  float z_gyro = rot.gyro.z;

  float temps = temp.temperature;

  //read altimeter values
  float alt = altimeter.readAltitude();
  float pressure = altimeter.readPressure();

  //store sensor values in array
  float sensors_v[]={alt, pressure, x_acc, y_acc, z_acc, x_gyro, y_gyro, z_gyro, temps};

  //write data to flash memory
  unsigned long address = 0;
  while (address < 9){
    flash.writeFloat(address,sensors_v[address]);
    Serial.print("Data recorded : ");
    Serial.print(sensors_v[address]);
    Serial.print(", At address: ");
    Serial.print(address);
    address++;  
    }
  Serial.println("Done writing to Flash!");

  //read data from flash memory
  Serial.println("Reading Flash...");
  int j = 0;
  while (j < 9){
    Serial.print("Data written at flash: ");
    Serial.println(flash.readFloat(j));
    j++;
  }
  
  delay(500);

  while(1); 

}
