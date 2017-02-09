#include "webserver.h"
#include "webserver_options.h"
#include <utility>
#include <map>
#include <numeric>
#include <boost/asio.hpp>




std::string Webserver::ToString() const{
  std::string webserver_string;
  webserver_string.append("port: " + std::to_string(port_) + " \n");
  for(const auto& option : options_) {
    webserver_string.append("key: ");
    webserver_string.append(option.first);
    webserver_string.append(", value: ");
    webserver_string.append(option.second.ToString());
    webserver_string.append("\n");
  }
  return webserver_string;
}

boost::system::error_code Webserver::port_valid() {
  // Based off of: 
  // http://stackoverflow.com/questions/33358321/using-c-and-boost-or-not-to-check-if-a-specific-port-is-being-used
  using namespace boost::asio;
  using ip::tcp;

  io_service svc;
  tcp::acceptor a(svc);

  boost::system::error_code ec;
  a.open(tcp::v4(), ec) || a.bind({ tcp::v4(), (unsigned short)port_ }, ec);

  return ec;
}

bool Webserver::configure_server(const char* file_name) {
  if (!parser_->Parse(file_name, config_)) {
    printf("Invalid config file");
    return false;
  } 

  std::string port_str = "";

  if (!config_->find("port", port_str)) {
    printf("Config does not specify a port\n");
    return false;
  }

  port_ = std::atoi(port_str.c_str());

  boost::system::error_code ec = port_valid();
  if(ec.value() != boost::system::errc::success) {
    printf("Unable to bind port %d because %s\n", port_, ec.message().c_str());
    return false;
  }

  std::vector<std::shared_ptr<NginxConfigStatement> > statements = 
    config_->findAll("location");

  for (const auto& statement : statements) {
    // printf("%s", statement->ToString(0).c_str());
    if(statement->tokens_.size() != 2) {
      // lazy way to stringify the vector of strings
      printf("Invalid config %s", std::accumulate(statement->tokens_.begin(), statement->tokens_.end(), std::string("")).c_str());
      return false;
    }
    
    std::map<std::string, std::vector<std::string> >* options = new std::map<std::string, std::vector<std::string> >;
    // exampple of a statement
    // location /static2 {
    //      root nginx-configparser;
    //  }
    //token[0] = location, token[1] = /static, childblock = root nginx-configparser;

    WebserverOptions opt(statement->child_block_, options);
    printf("%s\n", opt.ToString().c_str());
    //pair: /static -> root nginx-configparser;
    
    options_.insert(std::make_pair(statement->tokens_[1], opt));
  }
  return true;
}

bool Webserver::run_server(const char* file_name) {
  if(!configure_server(file_name))  {
    // Server cannot run
    return false;
  }
  try {  
    boost::asio::io_service io_service;
    Server s(io_service, port_, &options_);
    printf("Running server on port %d...\n", port_);
    io_service.run();
  } 
  catch (std::exception& e) {
    printf("Exception %s\n", e.what());
    return false;
  }
  return true;
}
