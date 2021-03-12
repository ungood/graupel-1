#include <Arduino.h>
#include <Indicators.h>
#include <unity.h>

Indicators indicators(PIN_OK, PIN_TX, PIN_ERROR);

void test_all_lights_off() {
  TEST_ASSERT_EQUAL(LOW, digitalRead(PIN_OK));
  TEST_ASSERT_EQUAL(LOW, digitalRead(PIN_TX));
  TEST_ASSERT_EQUAL(LOW, digitalRead(PIN_ERROR));
}

void test_on_is_always_on() {
  int period = 17;  // Purposefully pick a number not divisible by 16.
  indicators.ok.blink(period, 0xFFFF);
  for (int ms = 0; ms < period * 2; ms++) {
    indicators.loop(ms);
    TEST_ASSERT_EQUAL(HIGH, digitalRead(PIN_OK));
  }

  indicators.ok.off();
}

void test_blink_works() {
  int period = 16;
  indicators.tx.blink(period, 0x5555);

  for (int ms = 0; ms < period * 2; ms++) {
    indicators.loop(ms);
    TEST_ASSERT_EQUAL(ms % 2 == 0, digitalRead(PIN_TX));
  }

  indicators.tx.off();
}

void setup() {
  UNITY_BEGIN();
  indicators.setup();

  RUN_TEST(test_all_lights_off);
  RUN_TEST(test_on_is_always_on);
  RUN_TEST(test_blink_works);

  UNITY_END();
}

void loop() {}
