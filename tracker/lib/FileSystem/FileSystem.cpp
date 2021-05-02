#include <Arduino.h>
#include <ArduinoLog.h>

#include "FileSystem.h"

bool FileSystem::begin() {
  if(sd_.begin(config_)) {    
    return true;
  } else {
    Log.error(F("Error initializing SD card! Code: %X, Data: %X\n"), sd_.sdErrorCode(), sd_.sdErrorData());
    return false;
  }
}