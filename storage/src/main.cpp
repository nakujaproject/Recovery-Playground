#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"
#include "vecs.h"
#include "spifiles.h"

void writeFile(){
  Serial.printf("Writing file: %s\r\n", "/vecs.txt");

  File file = SPIFFS.open("/log.csv", FILE_WRITE);
  if(!file){
      Serial.println("- failed to open file for writing");
      return;
  }
  if(file.print("This is a test\n")){
      Serial.println("- file written");
  } else {
      Serial.println("- frite failed");
  }
  file.close();
}

void readFile(){
  Serial.printf("Reading file: %s\r\n", "/log.txt");

  File file = SPIFFS.open("/log.csv");
  if(!file || file.isDirectory()){
      Serial.println("- failed to open file for reading");
      return;
  }

  Serial.println("- read from file:");
  while(file.available()){
      Serial.write(file.read());
  }
  file.close();
}

void appendFile(){
  Serial.printf("Appending to file: %s\r\n", "/vecs.txt");

  File file = SPIFFS.open("/main.txt", FILE_APPEND);
  if(!file){
      Serial.println("- failed to open file for appending");
      return;
  }
  if(file.print("Fuck society\n")){
      Serial.println("- message appended");
  } else {
      Serial.println("- append failed");
  }
  file.close();
}

void dleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

void setup() {
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
  // setupSP();
  // writeFile();
  // appendFile();
//   dleteFile(SPIFFS, "/log.csv");
}

void test(){
  char ch;
  Serial.printf("Reading file: %s\r\n", "/log.csv");

  File file = SPIFFS.open("/log.csv");
  if(!file || file.isDirectory()){
      Serial.println("[-] failed to open file for reading");
      return;
  }

  Serial.println("- read from file:");
  while(file.available()){
      Serial.print("Current position: ");
      Serial.println(file.position());
      ch = file.read();
      if(ch == '\n'){
          delay(2000); // Pause for 2 seconds
          Serial.println();
      } else {
          Serial.write(ch);
      }
  }
  file.close();
}

void loop() {
    readFile();
    delay(10000);
  Serial.print("#");
}