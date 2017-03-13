#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cloud_file_handler.h"

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

//This in commented out because for some reason pip install boto3 didn't work on Travis build
// TEST(CloudFileHandler, HandleRequestTexT){
//   MockNginxConfig mock_config;
//   std::vector<std::string> set_tokens;
//   set_tokens.push_back("bucket");
//   set_tokens.push_back("cs130-s3");
//   EXPECT_CALL(mock_config, mocked_find("bucket"))
//     .WillOnce(
//       Return(set_tokens)  
//     );  
//     CloudFileHandler handler;
//     RequestHandler::Status ret = handler.Init("/cloud", mock_config);
    
//     EXPECT_EQ(ret, RequestHandler::OK);
//     std::string raw_req = "GET /cloud/testing.txt HTTP/1.1\r\nAccept-Languages: en-us\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: length\r\n\r\n";
//     auto req = Request::Parse(raw_req);

//     Response resp;
//     auto handler_status = handler.HandleRequest(*req,&resp);

//     EXPECT_EQ(handler_status,RequestHandler::OK);
//     EXPECT_EQ(resp.ToString(),"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nG0 = 2T\nP0 = 2T");
// }

TEST(CloudFileHandler, HandleRequestNotFound){
  MockNginxConfig mock_config;
  std::vector<std::string> set_tokens;
  set_tokens.push_back("bucket");
  set_tokens.push_back("cs130-s3");
  EXPECT_CALL(mock_config, mocked_find("bucket"))
    .WillOnce(
      Return(set_tokens)  
    );  
    CloudFileHandler handler;
    RequestHandler::Status ret = handler.Init("/cloud", mock_config);
    
    EXPECT_EQ(ret, RequestHandler::OK);
    std::string raw_req = "GET /cloud/testing.jpg HTTP/1.1\r\nAccept-Languages: en-us\r\nAccept-Encoding: gzip, deflate\r\nContent-Length: length\r\n\r\n";
    auto req = Request::Parse(raw_req);

    Response resp;
    auto handler_status = handler.HandleRequest(*req,&resp);

    EXPECT_EQ(handler_status,RequestHandler::FILE_NOT_FOUND);
    EXPECT_EQ(resp.ToString(),"HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n");
}
