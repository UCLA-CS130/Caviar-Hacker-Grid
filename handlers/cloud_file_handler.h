#ifndef CLOUD_FILE_HANDLER_H
#define CLOUD_FILE_HANDLER_H

#include "request_handler.h"

#include <string>

#include "../nginx-configparser/config_parser.h"
#include "../http/httpRequest.h"
#include "../http/httpResponse.h"
#include "not_found_handler.h"

class CloudFileHandler: public RequestHandler{
public:
  
  //it assumes that config is the block for this handler only, more specifically:
  //     path /cloud CouldFileHandler{
  //       bucket bucket_name;
  //     }
  //config should contains the "bucket bucket_name;"
  //uri_prefix in this case should be /cloud
  virtual Status Init(const std::string& uri_prefix, const NginxConfig& config);
  virtual Status HandleRequest(const Request& request, Response* response);
private:
  std::string generateName(std::string file_prefix, std::string extension);

  std::string m_bucket_name_;
  std::string m_uri_prefix_;
  NotFoundHandler NotFoundHandler_;
  //count how many files have been downloaded, also used to generate unique download file names
  static long m_downloads_counter;

};

REGISTER_REQUEST_HANDLER(CloudFileHandler);
#endif


