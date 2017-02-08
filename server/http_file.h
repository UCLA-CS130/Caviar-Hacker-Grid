#ifndef HTTP_FILE_H
#define HTTP_FILE_H

#include "http.h"

namespace http {
  class HTTPResponseBuilderFile : public http::HTTPResponseBuilder {
  public:
    HTTPResponseBuilderFile(HTTPResponse* res, std::string buf);
  };
}


#endif // If Guard