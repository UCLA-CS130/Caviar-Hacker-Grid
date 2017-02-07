#include "fileService.h"
#include "gtest/gtest.h"
#include "httpRequest.h"

TEST(FileServiceTest, BasicTest){
  HttpRequest parser;
  std::string request = "GET static/feline.txt HTTP/1.1\r\n\r\nHost: localhost:8080";
  parser.Parse(request);
  FileServicer fsr(parser);
  bool res = fsr.tryOpen();
  fsr.closeFs();
  EXPECT_TRUE(res);
}
