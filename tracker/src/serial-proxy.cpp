// This simple program uses the Arduino as a simple pass through from Serial to Serial3 and vice versa.
// This is useful for reading from and writing to the GPS module directly - which allows us to diagnose
// and configure the U-Blox GPS with the U-Center software.

#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  while(!Serial) {}
  
  Serial3.begin(9600);
  while(!Serial3) {}
}

void loop() {
  while(Serial3.available()) {
    Serial.write(Serial3.read());
  }

  while(Serial.available()) {
    Serial3.write(Serial.read());
  }
}