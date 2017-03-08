
// based off of: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp


#ifndef SERVER_H

#define SERVER_H

#include <vector>
#include <string>
#include <map>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "../http/httpRequest.h"
#include "../http/http.h"
#include "../handlers/request_handler.h"

using boost::asio::ip::tcp;

class ServerStatus;

class Session
{
public:
  Session(boost::asio::io_service& io_service, HandlerConfiguration* handler, ServerStatus* status);
  void start();
  tcp::socket& getSocket();
private:
  void do_read();

  void do_write(const std::string resp);

  tcp::socket socket_;
  std::string data_;
  HandlerConfiguration* handler_;
  ServerStatus* status_;
  boost::asio::streambuf stream_buffer_;
};

class Server 
{
public:
  Server(boost::asio::io_service& io_service, int port, HandlerConfiguration* handler, ServerStatus* status);
  void run();
private:
  void do_accept(HandlerConfiguration* handler, ServerStatus* status,Session* new_session, const boost::system::error_code& error);
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  std::size_t NUM_OF_THREADS = 4;
  boost::asio::io_service& io_service_;
};


class ServerStatus {
public:
  struct Status {
    Status(){
      printf("Mutex Created.\n");
      stsMx = new std::mutex;			
    }
    void destroyMutex(){
      delete stsMx;
    }
    std::map<std::string, int> RequestCountByURL_;
    std::map<int, int> ResponseCountByCode_;
    std::map<std::string, std::string> RequestHandlers_;

    std::mutex* stsMx;

    std::string defaultHandler_;
    int requests_ = 0;
  };

  void AddHandler(std::string path, std::string handler);
  Status GetStatus(); 
  void LogIncomingRequest(std::string path, int RespCode);
  void SetDefaultHandler(std::string handler);

private:
  Status Status_;
};
#endif // If Guard
