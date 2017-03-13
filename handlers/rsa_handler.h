#ifndef RSA_HANDLER_H
#define RSA_HANDLER_H

#include <string>

#include "../nginx-configparser/config_parser.h"
#include "../http/httpRequest.h"
#include "../http/httpResponse.h"
#include "request_handler.h"

class RsaHandler : public RequestHandler {
 public:
  Status Init(const std::string& uri_prefix,
	      const NginxConfig& config);

  bool isPrime(long long x){
    int sqrtX = std::sqrt(x);
    for (int i = 2; i < sqrtX; i++) 
      if (x % i == 0)
	return false;
    return true;
  }

  //Extended Eucliden algorithm used to determine d
  //see : http://www.geeksforgeeks.org/basic-and-extended-euclidean-algorithms
int gcdExtended(int a, int b, int *x, int *y)
{
    // Base Case
    if (a == 0)
    {
        *x = 0;
        *y = 1;
        return b;
    }
 
    int x1, y1; // To store results of recursive call
    int gcd = gcdExtended(b%a, a, &x1, &y1);
 
    // Update x and y using results of recursive
    // call
    *x = y1 - (b/a) * x1;
    *y = x1;
 
    return gcd;
}
  
  //Handle Request encrypts if the first letter of the 
  //request body is e, and decrypts if it is d, fails otherwise.
  //The 'message' in both cases is the rest of the message. 
  Status HandleRequest(const Request& request,
		       Response* response);

 private:
  long long p, q, n, t, e, d;
  std::string m;
  //p, q are initial prime numbers. n, the product of p and q, is the modulus.
  //t is Euler's Totient, computed as (n-1)(p-1).
  //e and d are the public and private exponents used in the modular arithmetic
  //m is the input message.

};

REGISTER_REQUEST_HANDLER(RsaHandler);

#endif
