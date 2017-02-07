#ifndef FILESERVICE_H
#define FILESERVICE_H

#include "httpRequest.h"
#include <iostream>
#include <fstream>
#include <string>
//TODO: Replace below by using http-refactor
#include <string>

class FileServicer
{
public:

  FileServicer(std::string s);
    //Sets member variable 'path'

  std::string tryOpen();
    // Safely attempts to open file into string,
    // and returns False upon error
  
  bool closeFs();
    // Closes the member filestream, to be used after tryOpen.
    // Returns False upon error (file not open, could not close, etc.)
  
private:
  std::string path;
  std::string pathPointer;
  std::string content;
  std::ifstream fs;

};
#endif
