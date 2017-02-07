#ifndef FILESERVICE_H
#define FILESERVICE_H

#include "./server/httpRequest.h"
#include <iostream>
#include <fstream>
//TODO: Replace below by using http-refactor
#include <string>

class FileServicer
{
 public:

  FileServicer(HttpRequest hreq);
    //Sets member variable 'path' according to information in Http Request

  bool tryOpen();
    // Safely attempts to open file into member variable fs,
    // and returns False upon error

  bool closeFs();
    // Closes the member filestream, to be used after tryOpen.
    // Returns False upon error (file not open, could not close, etc.)
  
private:
  const char* pathPointer;
  std::ifstream fs;

};
#endif
