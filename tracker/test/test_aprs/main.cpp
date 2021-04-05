#include <Arduino.h>
#include <APRS.h>
#include <unity.h>

void test_callsign_encoding() {
  char actual[7];
  ax25_encode_callsign("KJ7VPV", 0, actual);
  unsigned char expected1[7] = {0x96, 0x94, 0x6E, 0xAC, 0xA0, 0xAC, 0x60};
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected1, actual, 7);


  ax25_encode_callsign("SEA7", 1, actual);
  unsigned char expected2[7] = {0xA6, 0x8A, 0x82, 0x6e, 0x40, 0x40, 0x62};
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected2, actual, 7);
}

void test_base91_encoding() { 
  char actual[3];
  ax25_encode_base91(10061, actual, 3);

  char expected[] = {'"', '4', 'T'};
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, actual, 3);
}

void test_latitude_encoding() { 
  char actual[4];
  ax25_encode_latitude(49.5, actual, 4);

  char expected[] = {'5', 'L', '!', '!'};
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, actual, 4);
}

void test_longitude_encoding() { 
  char actual[4];
  ax25_encode_longitude(-72.75, actual, 4);

  char expected[] = {'<', '*', 'e', '7'};
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, actual, 4);
}

void test_altitude_encoding() { 
  char actual[2];
  ax25_encode_altitude(10004, actual, 2);

  char expected[] = {'S', ']'};
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, actual, 2);
}

void test_message_encoding() {
  CompressedPositionMessage message{"KJ7VPV", 0, 0, 0, APRS_MAC};
  message.update_fix(49.5, -72.75, 10004);

  char* buffer[sizeof(CompressedPositionMessage)];
  memcpy(buffer, &message, sizeof(CompressedPositionMessage));

  unsigned char expected[] = {
    0xA6, 0x8A, 0x82, 0x6e, 0x40, 0x40, 0x60, // SEA7  0
    0x96, 0x94, 0x6E, 0xAC, 0xA0, 0xAC, 0x60  // KJ7VPV0
  };

  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, buffer, 14);
}

void setup() {
  UNITY_BEGIN();

  RUN_TEST(test_callsign_encoding);
  RUN_TEST(test_base91_encoding);
  RUN_TEST(test_latitude_encoding);
  RUN_TEST(test_longitude_encoding);
  RUN_TEST(test_altitude_encoding);

  RUN_TEST(test_message_encoding);

  UNITY_END();
}

void loop() {}
