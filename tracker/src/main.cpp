#include <Arduino.h>
#include <SPI.h>
#include <StateMachine.h>

#include "sensors/bme280.h"
#include "utilities.h"

struct {
  // The number of milliseconds since boot, cached at the beginning of each loop.
  unsigned long millis = millis();
} tracker;

auto stateMachine = StateMachine(initialState);

BME280 bme;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  if(!bme.begin()) {
    fail("Uh oh!"); // TODO: use F macro
  }

  stateMachine.setup();
}

void loop() {
  tracker.millis = millis();
  delay(1000);
}
