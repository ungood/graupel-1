#ifndef TRACKER_H
#define TRACKER_H

#include <WString.h>

/**
 * Forces a hard reset of the device by jumping to address 0.
 */
void(* reset) (void) = 0;

/**
 * Something bad has happened.
 */ 
void abort(const int errorCode, const __FlashStringHelper* errorMessage);

#endif