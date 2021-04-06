#include <Arduino.h>

#include "FileSystem.h"

bool FileSystem::begin() {
  // keep the hardware SS pin (53 on Mega) set to output or the SD library won't work
  pinMode(PIN_SPI_SS, OUTPUT);
  if(sd_.begin(config_)) {

    sd_.ls(LS_SIZE | LS_R | LS_DATE);
    sd_.open("foo.txt", O_WRITE);
    return true;
  } else {
    return false;
  }
}