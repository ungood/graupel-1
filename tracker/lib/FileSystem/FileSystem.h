#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <SdFat.h>

class FileSystem {
  private:
    SdFat32 sd;
    File32 logFile;
    File32 dataFile;
  
  public:
    bool begin();
};

#endif