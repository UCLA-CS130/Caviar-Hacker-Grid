#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "markdown_handler.h"

#include <vector>

#include "../http/httpRequest.h"
#include "../http/httpResponse.h"
#include "../nginx-configparser/config_parser.h"


using ::testing::Return;
using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::An;

class MockNginxConfig : public NginxConfig{
public:
  std::vector<std::string> find(const std::string& key) const {
    return mocked_find(key);
  }
  MOCK_CONST_METHOD1(mocked_find,std::vector<std::string>(const std::string& key));
};

// Verify that old behavior remains consistent
TEST(MarkdownHandlerTest, HandleRequestText){
  MockNginxConfig mock_config;
  std::vector<std::string> set_tokens;
  set_tokens.push_back("root");
  set_tokens.push_back("files_served");
  EXPECT_CALL(mock_config, mocked_find("root"))
    .WillOnce(
      Return(set_tokens)  
    );  
  MarkdownHandler handler;
  RequestHandler::Status ret = handler.Init("/static1", mock_config);
  
  EXPECT_EQ(ret, RequestHandler::OK);
  std::string raw_req = "GET /static1/text_file.txt HTTP/1.1\r\nAccept-Languages: en-us\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: length\r\n\r\n";
  auto req = Request::Parse(raw_req);

  Response resp;
  auto handler_status = handler.HandleRequest(*req,&resp);

  EXPECT_EQ(handler_status,RequestHandler::OK);
  EXPECT_EQ(resp.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n# Caviar-Hacker-Grid\nCS130 Winter 2017 Webserver Project\n");
}

TEST(MarkdownHandlerTest, HandleRequestJPG){
  MockNginxConfig mock_config;
  std::vector<std::string> set_tokens;
  set_tokens.push_back("root");
  set_tokens.push_back("files_served");
  EXPECT_CALL(mock_config, mocked_find("root"))
    .WillOnce(
      Return(set_tokens)  
    );  
  MarkdownHandler handler;
  RequestHandler::Status ret = handler.Init("/stat", mock_config);
  
  EXPECT_EQ(ret, RequestHandler::OK);
  std::string raw_req = "GET /stat/jpg_file.jpg HTTP/1.1\r\nAccept-Languages: en-us\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: length\r\n\r\n";
  auto req = Request::Parse(raw_req);

  Response resp;
  auto handler_status = handler.HandleRequest(*req,&resp);

  EXPECT_EQ(handler_status,RequestHandler::OK);
  EXPECT_EQ(resp.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\n\r\n# Caviar-Hacker-Grid\nCS130 This is an image");
}

TEST(MarkdownHandlerTest, WrongPrefixPos){
  MockNginxConfig mock_config;
  std::vector<std::string> set_tokens;
  set_tokens.push_back("root");
  set_tokens.push_back("files_served");
  EXPECT_CALL(mock_config, mocked_find("root"))
    .WillOnce(
      Return(set_tokens)  
    );  
  MarkdownHandler handler;
  RequestHandler::Status ret = handler.Init("/stat", mock_config);
  
  EXPECT_EQ(ret, RequestHandler::OK);
  std::string raw_req = "GET /jpg_file.jpg/stat HTTP/1.1\r\nContent-Length: length\r\n\r\n";
  auto req = Request::Parse(raw_req);

  Response resp;
  auto handler_status = handler.HandleRequest(*req,&resp);

  EXPECT_EQ(handler_status,RequestHandler::BAD_REQUEST);
}

TEST(MarkdownHandlerTest, NoPrefix){
  MockNginxConfig mock_config;
  std::vector<std::string> set_tokens;
  set_tokens.push_back("root");
  set_tokens.push_back("files_served");
  EXPECT_CALL(mock_config, mocked_find("root"))
    .WillOnce(
      Return(set_tokens)  
    );  
  MarkdownHandler handler;
  RequestHandler::Status ret = handler.Init("/stat", mock_config);
  
  EXPECT_EQ(ret, RequestHandler::OK);
  std::string raw_req = "GET /jpg_file.jpg HTTP/1.1\r\nContent-Length: length\r\n\r\n";
  auto req = Request::Parse(raw_req);

  Response resp;
  auto handler_status = handler.HandleRequest(*req,&resp);

  EXPECT_EQ(handler_status,RequestHandler::BAD_REQUEST);

  EXPECT_EQ(resp.ToString(),"HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n");
}

TEST(MarkdownHandlerTest, NotFound){
  MockNginxConfig mock_config;
  std::vector<std::string> set_tokens;
  set_tokens.push_back("root");
  set_tokens.push_back("files_served");
  EXPECT_CALL(mock_config, mocked_find("root"))
    .WillOnce(
      Return(set_tokens)  
    );  
  MarkdownHandler handler;
  RequestHandler::Status ret = handler.Init("/static1", mock_config);
  
  EXPECT_EQ(ret, RequestHandler::OK);
  std::string raw_req = "GET /static1/text_f.txt HTTP/1.1\r\nContent-Length: length\r\n\r\n";
  auto req = Request::Parse(raw_req);

  Response resp;
  auto handler_status = handler.HandleRequest(*req,&resp);

  EXPECT_EQ(handler_status,RequestHandler::FILE_NOT_FOUND);
  EXPECT_EQ(resp.ToString(),"HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n");
}


// Verify that markdown is properly transformed

TEST(MarkdownHandlerTest, HandleRequestMarkdown){
  MockNginxConfig mock_config;
  std::vector<std::string> set_tokens;
  set_tokens.push_back("root");
  set_tokens.push_back("files_served");
  EXPECT_CALL(mock_config, mocked_find("root"))
    .WillOnce(
      Return(set_tokens)  
    );  
  MarkdownHandler handler;
  RequestHandler::Status ret = handler.Init("/static1", mock_config);
  
  EXPECT_EQ(ret, RequestHandler::OK);
  std::string raw_req = "GET /static1/README.md HTTP/1.1\r\nAccept-Languages: en-us\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: length\r\n\r\n";
  auto req = Request::Parse(raw_req);

  Response resp;
  auto handler_status = handler.HandleRequest(*req,&resp);

  EXPECT_EQ(handler_status,RequestHandler::OK);
  EXPECT_EQ(resp.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: html\r\n\r\n<p><a href=\"https://travis-ci.org/UCLA-CS130/Caviar-Hacker-Grid.svg?branch=master\">![Build Status</a>](https://travis-ci.org/UCLA-CS130/Caviar-Hacker-Grid)</p>\n\n<h1>Caviar-Hacker-Grid</h1>\n<p>CS130 Winter 2017 Webserver Project</p>\n\n<p>Commands are to be run from the base Caviar-Hacker-Grid directory.</p>\n\n<h1>Installation</h1>\n<p>You must clone the repository recursively. <code>` git clone --recursive https://github.com/UCLA-CS130/Caviar-Hacker-Grid.git `</code></p>\n\n<p>To run the server, cd into the directory and run  <code>`make &amp;&amp; ./webserver test_config`</code></p>\n\n");
}