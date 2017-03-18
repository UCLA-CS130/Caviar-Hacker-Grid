#include "fileService.h"
#include "./server/httpRequest.h"
#include <string>
#include <fstream>

FileServicer::FileServicer(HttpRequest hreq){
  pathPointer = hreq.getResourcePath().c_str();
}

bool FileServicer::tryOpen(){
  this->fs.open(pathPointer, std::ifstream::in);
  if(fs.good()){
    printf("File opening is good!");
    return true;
  }
}

bool FileServicer::closeFs(){
  this->fs.close();
  return true;
}
