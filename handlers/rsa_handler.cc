#include "rsa_handler.h"

#include <string>
#include <iostream>

//For the crypto-secure rng seeding
#include <boost/nondet_random.hpp>


#include "../http/httpRequest.h"
#include "../http/httpResponse.h"
#include "../http/http.h"

//The init process generates the two numbers p and q,
//and initializes the other member variables using p and q's values.
RequestHandler::Status RsaHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
  //Todo
  //  Generate p and q using random num generator
  //  boost::random_device rd;
  // p = rd();
  //p = (p << 32) | rd();
  //q = rd();
  //q = (q << 32) | rd();
  p = 11;
  q = 13;
  n = p*q;
  t = (p-1)*(q-1);
  //Now we calculate e and d
  //We hardcode e as is common practice, using a number
  //of possible e's, as e must be relatively prime to t.
  //Else, we start incrementing e until a found number works.
  //Thi is method used by GnuPG v1.2.3
  if(t % 41 != 0)
    e = 41;
  else if(t % 257 != 0)
    e = 257;
  else{
    int temp1 = 0, temp2 = 0;
    e = 65537;
    while (gcdExtended(e, t, &temp1, &temp2) != 1)
      e += 2;
  }
  //Find d using EEA
  int x = 0, y = 0;
  gcdExtended(e,t,&x,&y);
  d = x;
  std::cout << "The initialization values for the RSA Handler:\n";
  std::cout << "n: " << n << ", t: " << t << ", e: " << e << ", d: " << d <<"\n";
  return OK;
}

//used as reference for below: http://www.sanfoundry.com/cpp-program-implement-rsa-algorithm/
RequestHandler::Status RsaHandler::HandleRequest(const Request& request,
						 Response* response) {
  m = (request.raw_request());
  std::size_t lengthm = m.length();
  long int encodedArray[128];
  std::string output = "";
  long int plain;
  long int cipher;
  std::size_t u;
  for(u = 0; u < lengthm; u++) {
    plain = m[u];
    //Conversion to number
    plain = plain - 96;
    long k = 1;
    for(long j = 0; j < e; j++) {
      k = k*plain;
      k = k % n;
    }
    cipher = k + 96;
    encodedArray[u] = cipher;
  }
  
  for(int temp = 0; temp < 128; temp++){
    output += std::to_string(encodedArray[temp]);
  }
  std::cout << "Encrypted Output:\n";
  std::cout <<output << "\n";
  response->SetStatus(Response::OK);
  response->AddHeader("Content-Type", http::mime_type::ContentTypeAsString(http::mime_type::CONTENT_TYPE_TEXT_PLAIN));
  response->SetBody(output.c_str());
  return OK; 
}
