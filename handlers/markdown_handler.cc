#include "markdown_handler.h"
#include "../markdown/markdown.h"
#include "not_found_handler.h"

#include <boost/algorithm/string.hpp> 
#include <string>
#include <sstream>

RequestHandler::Status MarkdownHandler::HandleRequest(const Request& request, Response* response) {
  std::string req_uri = request.uri();
  std::size_t prefix_pos = req_uri.find(this->m_uri_prefix_);

  //the server should check for this first, this is just for safety
  if (prefix_pos == std::string::npos || prefix_pos != 0){
    printf("StaticHandler.HandleRequest: Bad request");
    response->SetStatus(Response::BAD_REQUEST);
    response->AddHeader("Content-Type",http::mime_type::ContentTypeAsString(http::mime_type::CONTENT_TYPE_TEXT_HTML));
    return BAD_REQUEST;
  }

  std::string uri_no_prefix = req_uri.substr(this->m_uri_prefix_.size());
  std::string actual_uri = this->m_root_path_ + uri_no_prefix;

  if (!FileIO::FileExists(actual_uri)){
    printf("StaticHandler.HandleRequest: File not found:%s\n",actual_uri.c_str());
    return NotFoundHandler_.HandleRequest(request, response);
  }

  response->SetStatus(Response::OK);

  // find the file extension, set the content type based on this.
  std::size_t index_of_extension = FileIO::FileExtensionLocation(actual_uri);
  std::string file_extension = "";
  if (index_of_extension == std::string::npos){
    response->AddHeader("Content-Type",http::mime_type::ContentTypeAsString(http::mime_type::CONTENT_TYPE_APP_OCTET_STREAM));
  }
  else {
    file_extension = actual_uri.substr(index_of_extension + 1);
    boost::algorithm::to_lower(file_extension);
    if (file_extension != "md") {
      response->AddHeader("Content-Type", http::mime_type::ContentTypeAsString(http::mime_type::GetMimeType(file_extension)));
    }
    else {
      response->AddHeader("Content-Type","html");
    }
  }
  
  std::string body;
  FileIO::FileToString(actual_uri, body);
  if(file_extension == "md") {
    markdown::Document doc;
    doc.read(body);
    std::stringstream html_body;
    doc.write(html_body);
    response->SetBody(html_body.str());
  }
  else {
    response->SetBody(body);
  }

  return OK;
}