
// based off of: http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp
// http://think-async.com/asio/boost_asio_1_5_3/doc/html/boost_asio/example/local/stream_server.cpp


#include "server.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <numeric>
#include <vector>

#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "webserver.h"
#include "../http/http.h"
#include "../http/httpRequest.h"
#include "../http/httpResponse.h"
#include "../handlers/request_handler.h"

using boost::asio::ip::tcp;

Session::Session(boost::asio::io_service& io_service, HandlerConfiguration* handler, ServerStatus* status)
  : socket_(io_service),
  handler_(handler),
  status_(status) {
}

tcp::socket& Session::getSocket(){
  return this->socket_;
}

void Session::start() {
  boost::asio::async_read_until(socket_, stream_buffer_, "\r\n\r\n",
      boost::bind(&Session::do_read, this));
}

void Session::do_read() {
  std::string buffered_input{
    buffers_begin(stream_buffer_.data()),
    buffers_end(stream_buffer_.data())
  };
  std::unique_ptr<Request> req = Request::Parse(buffered_input);
  Response* response = new Response;
  std::string response_string;
  if(req.get()) {
          // response valid 
    printf("request key: %s\n", req->uri().c_str());
          
    std::string best_key = handler_->FindLongestHandlerKey(req->uri());
    RequestHandler::Status RequestStatus;

    if(best_key == "") {
            // use the default handler, as no key was found for the URI
      printf("no key found, 404ing\n");
      RequestStatus = handler_->DefaultHandler->HandleRequest(*req, response);
    }
    else {
      printf("key %s found\n", best_key.c_str());
      printf("key in config: %s\n", handler_->RequestHandlers->find(best_key)->first.c_str());
      // do_write(handler_->DefaultHandler);
      RequestStatus = handler_->RequestHandlers->find(best_key)->second->HandleRequest(*req, response);
    }
    

    if(RequestStatus == RequestHandler::BAD_REQUEST) {
      response_string = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\n\r\n";
      status_->LogIncomingRequest(req->uri(), 500);
    }
    else {
      response_string = response->ToString();
      status_->LogIncomingRequest(req->uri(), response->GetStatus());
    }
  } 
  else {
    // response invalid, return a 400
    printf("$$$$$$$$$$$$$$$%s##############\n", data_.c_str());
    response_string = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n";
    status_->LogIncomingRequest(req->uri(), 400);
  }

  do_write(response_string);
  delete response;
}

void Session::do_write(const std::string resp) {
  //no idea how this works...
  boost::asio::streambuf out_streambuf;
  std::ostream out(&out_streambuf);
  out << resp;
  boost::asio::write(socket_, out_streambuf);
  tcdrain(socket_.native_handle());

  boost::system::error_code ec;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  socket_.close();
}


Server::Server(boost::asio::io_service& io_service, int port, HandlerConfiguration* handler, ServerStatus* status)
  : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
  socket_(io_service),
  io_service_(io_service){
  Session* new_session = new Session(this->io_service_,handler,status);
  acceptor_.async_accept(new_session->getSocket(),
      boost::bind(&Server::do_accept, this, handler, status, new_session, boost::asio::placeholders::error)
      );

}

void Server::do_accept(HandlerConfiguration* handler, ServerStatus* status,Session* new_session, const boost::system::error_code& error)
{
  if(!error){
    new_session->start();
    new_session = new Session(this->io_service_,handler,status);
    acceptor_.async_accept(new_session->getSocket(),
      boost::bind(&Server::do_accept, this, handler, status, new_session, boost::asio::placeholders::error)
      );
  }
  else{
    delete new_session;
  }

}

void Server::run(){
  std::vector<boost::shared_ptr<boost::thread>> threads;
  for (std::size_t i = 0; i < NUM_OF_THREADS; i++){
    boost::shared_ptr<boost::thread> thread(new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
    threads.push_back(thread);
  }
  for (std::size_t i = 0; i < threads.size();i++){
    threads[i]->join();
  }
}

void ServerStatus::AddHandler(std::string path, std::string handler) {
  Status_.RequestHandlers_.insert(std::make_pair(path, handler));
}

ServerStatus::Status ServerStatus::GetStatus() {
  return Status_;
}

void ServerStatus::LogIncomingRequest(std::string url, int RespCode) {
  auto RespCodeCount = Status_.ResponseCountByCode_.insert(std::make_pair(RespCode, 1));
  if (RespCodeCount.second == false) { // The particular response code is already in the map
    RespCodeCount.first->second++;
  }

  auto URLCodeCount = Status_.RequestCountByURL_.insert(std::make_pair(url, 1));
  if (URLCodeCount.second == false) { // The particular url is already in the map
    URLCodeCount.first->second++;
  }

  Status_.requests_++;
}

void ServerStatus::SetDefaultHandler(std::string handler) {
  Status_.defaultHandler_ = handler;
}
