#ifndef MARKDOWN_HANDLER_H
#define MARKDOWN_HANDLER_H

#include "request_handler.h"

#include <string>

#include "../nginx-configparser/config_parser.h"
#include "../http/httpRequest.h"
#include "../http/httpResponse.h"
#include "../filesystem/file_opener.h"
#include "file_handler.h"
#include "not_found_handler.h"


class MarkdownHandler : public StaticHandler {

public:
  virtual Status HandleRequest(const Request& request, Response* response);

};

REGISTER_REQUEST_HANDLER(MarkdownHandler);


#endif

