#ifndef UTILITIES_H
#define UTILITIES_H

#include <Arduino.h>

/**
 * Forces a hard reset of the device by jumping to address 0.
 */
void(* reset) (void) = 0;

/**
 * Something bad has happened.
 */ 
void abort(char* message) {
  Serial.println(message);
  delay(3000);
  reset();
}

#endif