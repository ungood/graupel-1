#include <Arduino.h>
#include <unity.h>
#include <config.h>

void test_sensors_begin() {
  TEST_ASSERT_TRUE(sensors.begin());
}

SensorReading reading;

void test_sensors_refresh() {
  TEST_ASSERT_TRUE(sensors.read(reading));

  TEST_ASSERT_GREATER_THAN(0, reading.temperature);
  TEST_ASSERT_GREATER_THAN(0, reading.pressure);
  TEST_ASSERT_GREATER_THAN(0, reading.altitude);
  TEST_ASSERT_GREATER_THAN(0, reading.humidity);
}

void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_sensors_begin);

  for(int i = 0; i < 20; i++) {
    RUN_TEST(test_sensors_refresh);
    delay(10);
  }
  
  UNITY_END();
}

void loop() {
}
