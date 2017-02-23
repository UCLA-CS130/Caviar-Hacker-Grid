#include 'request_handler.h'
#include 'http.h'
#include <string>

EchoHandler::Status Echohandler::Init(const std::string& uri_prefix,
				      const NginxConfig& config) {
  /*  m_uri_prefix_ = uri_prefix;
  auto config_tokens = config.find("echo");
  if(config_tokens.size() < 2) {
    printf("Invalid Config!\n");
    return BAD;
  }*/
  return OK;
}

EchoHandler::Status EchoHandler::HandleRequest(const Request& request,
					       Response* response) {
  response->SetStatus(Response::OK);
  response->AddHeader("Content-Type", http::mime_type::ContentTypeAsString(http::mime_type::CONTENT_TYPE_TEXT_PLAIN));
  response->SetBody(request->getMessageBody());

  return OK;
}
