#include "webserver.h"
#include "nginx-configparser/config_parser.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: ./webserver <path to config file>\n");
    return 1;
  }

  Nginx::NginxConfig config;

  if(!Nginx::ParseFile(argv[1], &config)) {
    printf("Syntax Error in NginxConfig %s", argv[1]);
    return 1;
  }

  Webserver server(&config);

  if(!server.run_server(argv[1])) {
    printf("Error running server.");
  	return 1;
  }
  
  return 0;
}


