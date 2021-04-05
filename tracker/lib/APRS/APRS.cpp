
/* From Project Swift - High altitude balloon flight software                 */
/*=======================================================================*/
/* Copyright 2010-2012 Philip Heron <phil@sanslogic.co.uk>               */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

/*
 * aprs symbol table:  http://ak6ak.net/Info/APRS%20Symbol%20Chart.html
 */

#include <Arduino.h>
#include <ArduinoLog.h>

#include <util/crc16.h>
#include <avr/pgmspace.h>
#include <stdarg.h>

#include "APRS.h"

const size_t FRAME_SIZE = 100;

#define BAUD_RATE      (1200)
#define TABLE_SIZE     (512)
#define PREAMBLE_BYTES (50)
#define REST_BYTES     (5)

#define APRS_PRE_EMPHASIS                     // Comment out to disable 3dB pre-emphasis.
#define PLAYBACK_RATE    (F_CPU / 256)
#define SAMPLES_PER_BAUD (PLAYBACK_RATE / BAUD_RATE)
#define PHASE_DELTA_1200 (((TABLE_SIZE * 1200L) << 7) / PLAYBACK_RATE)
#define PHASE_DELTA_2200 (((TABLE_SIZE * 2200L) << 7) / PLAYBACK_RATE)
#define PHASE_DELTA_XOR  (PHASE_DELTA_1200 ^ PHASE_DELTA_2200)

// Variables used by the timer interrupt
static uint8_t _enable_pin = 0;
volatile static byte *_txbuf = 0;
volatile static size_t _txlen = 0;
volatile static bool _is_transmitting = false;

void CompressedPositionMessage::set_position(float latitude, float longitude, float altitude_feet) {
  ax25_encode_latitude(latitude, compressed_lat, 4);
  ax25_encode_longitude(longitude, compressed_lng, 4);
  ax25_encode_altitude(altitude_feet, compressed_alt, 2);
}

char compression_type = 0b0011000 + 33; // Adding 33 converts this to a 'base-91' ASCII character.

bool APRS::begin() {
  Log.verbose("Initializing radio...\n");
  // Seed the PRNG with some noise.
  randomSeed(analogRead(A6));

  _enable_pin = enable_pin_; 
  pinMode(enable_pin_, OUTPUT);
  digitalWrite(enable_pin_, LOW);
  delay(25);

  // Fast PWM mode, non-inverting output on OC2A
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS20);  
  pinMode(data_pin_, OUTPUT);

  nextTransmission_ = millis();
  return true;
}

bool APRS::loop(unsigned long currentMillis) {
  if(currentMillis > nextTransmission_ && !_is_transmitting) {
    ax25_send_frame(&message_, sizeof(CompressedPositionMessage));
    // Add some random jitter (+/- 10 seconds to the next transmission to avoid interference if someone is transmitting at same interval).
    nextTransmission_ = currentMillis + random((transmissionDelay_ - 10) * 1000, (transmissionDelay_ + 10) * 1000);
    Log.trace(F("Next tranmission at %l\n"), nextTransmission_);
    return true;
  }
  return false;
}

void APRS::set_position(float latitude, float longitude, float altitude_feet) {
  message_.set_position(latitude, longitude, altitude_feet);

  // Above 10,000 ft, reduce path (to 0) and transmit every 5 minutes.
  if(altitude_feet > 10000) {
    message_.set_ttls(0, 0);
    transmissionDelay_ = 5 * 60;
  // Below, increase path and transmit more frequently.
  } else {
    message_.set_ttls(1, 1);
    transmissionDelay_ = 1 * 60;
  }
}

void APRS::set_comment(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  vsnprintf(message_.comment, MAX_COMMENT_LEN, fmt, va);
  va_end(va);
}

void ax25_send_frame(void *message, size_t message_len) {
  static uint8_t frame[FRAME_SIZE];
  uint8_t *s;
  uint16_t checksum;

  memset(frame, 0x00, FRAME_SIZE);
  memcpy(frame, message, message_len);
  
  /* Calculate and append the checksum */
  for(checksum = 0xFFFF, s = frame; *s; s++) {
    checksum = _crc_ccitt_update(checksum, *s);
  }

  *(s++) = ~(checksum & 0xFF);
  *(s++) = ~((checksum >> 8) & 0xFF);

  Log.verbose(F("Sending APRS Frame: %s\n"), frame);

  /* Point the interrupt at the data to be transmit */
  _txbuf = frame;
  _txlen = s - frame;
  _is_transmitting = true;

  /* Enable the timer and key the radio */
  TIMSK2 |= _BV(TOIE2);

  digitalWrite(_enable_pin, HIGH);
  delay(25);
}

// Magic!
ISR(TIMER2_OVF_vect) {
  static uint16_t phase  = 0;
  static uint16_t step   = PHASE_DELTA_1200;
  static uint16_t sample = 0;
  static uint8_t rest    = PREAMBLE_BYTES + REST_BYTES;
  static uint8_t byte;
  static uint8_t bit     = 7;
  static int8_t bc       = 0;
  uint8_t value;
  
  /* Update the PWM output */
  value = pgm_read_byte(&SINE_TABLE[(phase >> 7) & 0x1FF]);
  #ifdef APRS_PRE_EMPHASIS
  if (step == PHASE_DELTA_1200)
  {
    value = (value >> 1) + 64;
  }
  #endif
  OCR2B = value;
  phase += step;

  if(++sample < SAMPLES_PER_BAUD) return;
  sample = 0;

  /* Zero-bit insertion */
  if(bc == 5)
  {
    step ^= PHASE_DELTA_XOR;
    bc = 0;
    return;
  }

  /* Load the next byte */
  if(++bit == 8)
  {
    bit = 0;

    if(rest > REST_BYTES || !_txlen)
    {
      if(!--rest)
      {
        // Disable radio, disable interrupt
        if(_enable_pin) {
          digitalWrite(_enable_pin, 0);
          delay(25);
        }
        _is_transmitting = false;
        
        TIMSK2 &= ~_BV(TOIE2);

        /* Prepare state for next run */
        phase = sample = 0;
        step  = PHASE_DELTA_1200;
        rest  = PREAMBLE_BYTES + REST_BYTES;
        bit   = 7;
        bc    = 0;
        return;
      }

      /* Rest period, transmit ax.25 header */
      byte = 0x7E;
      bc = -1;
    }
    else
    {
      /* Read the next byte from memory */
      byte = *(_txbuf++);
      if(!--_txlen) rest = REST_BYTES + 2;
      if(bc < 0) bc = 0;
    }
  }

  /* Find the next bit */
  if(byte & 1)
  {
    /* 1: Output frequency stays the same */
    if(bc >= 0) bc++;
  }
  else
  {
    /* 0: Toggle the output frequency */
    step ^= PHASE_DELTA_XOR;
    if(bc >= 0) bc = 0;
  }

  byte >>= 1;
}

// Base-91 encodes the value in a buffer of given length.  Does NOT append null terminator.
void ax25_encode_base91(uint32_t value, char *buf, size_t buf_len) {
  for(size_t i = buf_len; i; i--) {
    buf[i-1] = value % 91 + 33;
    value /= 91;
  }
}

void ax25_encode_latitude(float latitude, char *buf, size_t buf_len) {
  uint32_t value = 380926 * (90 - latitude);
  ax25_encode_base91(value, buf, buf_len);
}

void ax25_encode_longitude(float longitude, char *buf, size_t buf_len) {
  uint32_t value = 190463 * (180+longitude);
  ax25_encode_base91(value, buf, buf_len);
}

// Encodes altitude as the "cs" bytes in a compressed position report.
static auto log_1002 = log(1.002);
void ax25_encode_altitude(float altitude_feet, char *buf, size_t buf_len) {
  uint32_t value = log(altitude_feet) / log_1002;
  ax25_encode_base91(value, buf, buf_len);
}

// Encodes a 6-byte (or less) callsign + ssid in 7 bytes.
void ax25_encode_callsign(const char *callsign, short ssid, char *buf) {
  char i;
  for(i = 0; i < 6; i++)
  {
    if(*callsign) *(buf++) = *(callsign++) << 1;
    else *(buf++) = ' ' << 1;
  }
  *(buf++) = ('0' + ssid) << 1;
}