#include "cloud_file_handler.h"

#include <vector>
#include <string.h>
#include <cstdio>
#include <mutex>

#include "../http/http.h"
#include "not_found_handler.h"
#include "../filesystem/file_opener.h"


#include <stdio.h>
#include <sys/wait.h>

long CloudFileHandler::m_downloads_counter = 0;
std::mutex download_counter_mutex;

RequestHandler::Status CloudFileHandler::Init(const std::string& uri_prefix, const NginxConfig& config){
  this->m_uri_prefix_ = uri_prefix;
  //assume the config argument has something like
  // bucket bucket_name;
  //check if such a line exists
  std::vector<std::string> cloud_config_tokens = config.find("bucket");
  if (cloud_config_tokens.size() < 2){
    printf("CloudFileHandler.Init: Invalid config:\n%s", config.ToString().c_str() );
    return INVALID_CONFIG;
  }

  this->m_bucket_name_ = cloud_config_tokens[1];
  return OK;
}

std::string CloudFileHandler::generateName(std::string file_prefix, std::string extension){
  std::lock_guard<std::mutex> lock(download_counter_mutex);
  this->m_downloads_counter++;
  return file_prefix + std::to_string(this->m_downloads_counter) + extension;

}

std::string s3_script_cmd = "./S3/s3_get_file.py";

RequestHandler::Status CloudFileHandler::HandleRequest(const Request& request, Response* response){
  std::string req_uri = request.uri();
  std::size_t prefix_pos = req_uri.find(this->m_uri_prefix_);

  //the server should check for this first, this is just for safety
  if (prefix_pos == std::string::npos || prefix_pos != 0){
    printf("CloudFileHandler.HandleRequest: Bad request");
    response->SetStatus(Response::BAD_REQUEST);
    response->AddHeader("Content-Type",http::mime_type::ContentTypeAsString(http::mime_type::CONTENT_TYPE_TEXT_HTML));
    return BAD_REQUEST;
  }


  std::string file_key = req_uri.substr(this->m_uri_prefix_.size()+1);
  std::size_t index_of_extension = FileIO::FileExtensionLocation(file_key);
  std::string file_extension = "";
  if (index_of_extension != std::string::npos){
  	file_extension = file_key.substr(index_of_extension + 1);
  }
  //download file from the cloud
  std::string download_name = generateName("remote_file",file_extension);
  //run the python script to access S3 storage
  std::string cmd_str = s3_script_cmd + " " + this->m_bucket_name_ + " " + file_key + " " + download_name;
  int ret = system(cmd_str.c_str());

  if(ret != 0){
  	printf("CloudFileHandler.HandleRequest: Error getting remote file:%s\n",file_key.c_str());
    return NotFoundHandler_.HandleRequest(request, response);
  }
  std::string body;
  response->SetStatus(Response::OK);
  FileIO::FileToString(download_name, body);
  response->SetBody(body);

  if(file_extension == ""){
  	response->AddHeader("Content-Type",http::mime_type::ContentTypeAsString(http::mime_type::CONTENT_TYPE_APP_OCTET_STREAM));
  }
  else{
  	response->AddHeader("Content-Type",http::mime_type::ContentTypeAsString(http::mime_type::GetMimeType(file_extension)));
  }
  //clean up
  std::string clean_up_cmd = "rm "+ download_name;
  int ret2 = system(clean_up_cmd.c_str());

  if(ret2 !=0 ){
    printf("CloudFileHandler.HandleRequest: Error cleaning up downloaded file\n");

  }

  return OK;
  
}