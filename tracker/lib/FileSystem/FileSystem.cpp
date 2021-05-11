#include <Arduino.h>
#include <ArduinoLog.h>

#include "FileSystem.h"

bool FileSystem::begin(Record &record) {
  if (!sd_.begin(config_)) {
    Log.error(F("Error initializing SD card! Code: %X, Data: %X\n"), sd_.sdErrorCode(), sd_.sdErrorData());
    return false;
  }

  uint16_t dirNumber = 0;
  char filename[9];
  do {
    snprintf(filename, 9, "%04X.csv", dirNumber);
    dirNumber++;
  } while (sd_.exists(filename));

  Log.trace(F("Creating data file: %s\n"), filename);

  if (!dataFile_.open(filename, O_WRITE | O_CREAT | O_TRUNC)) {
    Log.error(F("Failed to create data file: %s\n"), filename);
    return false;
  }

  record.writeHeader(dataFile_);
  return true;
}

void FileSystem::write(Record &record) {
  record.write(dataFile_);
}