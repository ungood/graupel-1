#include <Arduino.h>
#include <GPS.h>
#include <unity.h>

#include "config.h"

void test_gps_begin() {
  TEST_ASSERT_TRUE(gps.begin());
}

void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_gps_begin);

  UNITY_END();
}

void loop() {
  // TODO:
}
