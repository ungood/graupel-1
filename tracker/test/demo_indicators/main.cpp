#include <Arduino.h>
#include <Indicators.h>
#include <unity.h>

Indicators indicators(PIN_OK, PIN_TX, PIN_ERROR);

void setup() {
  UNITY_BEGIN();
  indicators.setup();

  const int period = 500;
  indicators.ok.blink(period, 0x000A);
  indicators.tx.blink(period, 0x00A0);
  indicators.error.blink(period, 0x0F00);
}

unsigned long currentMillis = millis();
const unsigned long endDemo = currentMillis + 10000;

void loop() {
  currentMillis = millis();

  indicators.loop(currentMillis);

  if (currentMillis > endDemo) {
    UNITY_END();
  }
}
