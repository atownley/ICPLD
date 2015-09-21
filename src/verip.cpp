/*
   This piece of code accepts an ip
   and which protocol family it belongs to
   and tries to resolve it in order to make
   sure the given IP is valid
*/
#include "cuc.h"

#include <netdb.h>
#include <sys/socket.h>

bool verip (std::string ip, int af)
{

  /* Linux supports getipnodebyname as well, but 
     since I'm unsure about the other OS's, this
     directive seems appropriate */
#ifndef SOLARIS 
  h_errno = 0;
  if (af == 4){
    if (gethostbyname2(ip.c_str(), AF_INET) != NULL)
      return true;
   
    else {
      if(h_errno == HOST_NOT_FOUND)
	cout << "Could not find IPv4 host " << ip << endl;
      else if (h_errno == NO_ADDRESS || h_errno == NO_DATA)
	cout << ip << " is valid, but lacks a corresponding ip address" << endl;
      else if (h_errno == TRY_AGAIN)
	cout << "Unable to obtain an answer from a DNS-server" << endl;
      else
	cout << "An error occured while verifying " << ip << endl;
    }
  }
  
  else if(af == 6) { 
    h_errno = 0;
    if (gethostbyname2(ip.c_str(),AF_INET6) != NULL)
      return true;
    else {
      if(h_errno == HOST_NOT_FOUND)
	cout << "Could not find IPv4 host " << ip << endl;
      else if (h_errno == NO_ADDRESS || h_errno == NO_DATA)
	cout << ip << " is valid, but lacks a corresponding ip address" << endl;
      else if (h_errno == TRY_AGAIN)
	cout << "Unable to obtain an answer from a DNS-server" << endl;
      else
	cout << "An error occured while verifying " << ip << endl;
    }      
      
  }
#endif
#ifdef SOLARIS

  int error_num;
  if (af==4) {
    if (getipnodebyname(ip.c_str(), AF_INET, 0, &error_num) != NULL)
      return true;
    else 
      cout << "This IPv4 address: " << ip << " is wrongly formatted." << endl;
  }
  else if (af == 6) {
    if (getipnodebyname(ip.c_str(), AF_INET6, 0, &error_num) != NULL)
      return true;
    else 
      cout << "This IPv6 address: " << ip << " is wrongly formatted." << endl;
  }
#endif
      
  return false;
}
