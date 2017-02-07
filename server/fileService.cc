#include "fileService.h"
#include "httpRequest.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>

FileServicer::FileServicer(std::string s){
  path = s;
}

std::string FileServicer::tryOpen(){
  std::cout << path;
  fs.open(path.c_str(), std::ifstream::in);
  printf("TESTING NOW\n\n");
  if(fs.is_open()){
    printf("File opening is good!");
    content.assign( (std::istreambuf_iterator<char>(fs) ),
		    (std::istreambuf_iterator<char>() ) );
    return content;
  }
  printf("File opening failed!\n");
  return("");
}

bool FileServicer::closeFs(){
  fs.close();
  printf("File Closed!\n");
  return true;
}
