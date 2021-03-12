#include <Arduino.h>

#include "FileSystem.h"

bool FileSystem::begin() {
  // keep the hardware SS pin (53 on Mega) set to output or the SD library won't work
  pinMode(PIN_SPI_SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  //return sd.begin(10, 11, 12, 13);
  return false;
}