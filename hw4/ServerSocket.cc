/*
 * Copyright ©2023 Justin Hsia.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2023 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.

#include "./ServerSocket.h"

#define BUF 1024

extern "C" {
  #include "libhw1/CSE333.h"
}

namespace hw4 {

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int* const listen_fd) {
  // Use "getaddrinfo," "socket," "bind," and "listen" to
  // create a listening socket on port port_.  Return the
  // listening socket through the output parameter "listen_fd"
  // and set the ServerSocket data member "listen_sock_fd_"

  // STEP 1:
  if (ai_family != AF_INET && ai_family != AF_INET6
      && ai_family != AF_UNSPEC) {
        return false;
      }
  // sock_family_ = ai_family;

  // Populate the "hints" addrinfo structure for getaddrinfo().
  // ("man addrinfo")
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6;       // IPv6 (also handles IPv4 clients)
  hints.ai_socktype = SOCK_STREAM;  // stream
  hints.ai_flags = AI_PASSIVE;      // use wildcard "in6addr_any" address
  hints.ai_flags |= AI_V4MAPPED;    // use v4-mapped v6 if no v6 found
  hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
  hints.ai_canonname = nullptr;
  hints.ai_addr = nullptr;
  hints.ai_next = nullptr;

  // get a string representation of our portnumber to
  // pass in to getaddrinfo().
  // getaddrinfo() returns a list of
  // address structures via the output parameter "result".
  std::string portNum = std::to_string(port_);
  struct addrinfo* result;
  int res = getaddrinfo(nullptr, portNum.c_str(), &hints, &result);

  // Did addrinfo() fail?
  if (res != 0) {
    std::cerr << "getaddrinfo() failed: ";
    return false;
  }

  // Loop through the returned address structures until we are able
  // to create a socket and bind to one.  The address structures are
  // linked in a list through the "ai_next" field of result.
  int listen_fd_new = -1;
  for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    listen_fd_new = socket(rp->ai_family,
                       rp->ai_socktype,
                       rp->ai_protocol);
    if (listen_fd_new == -1) {
      // Creating this socket failed.  So, loop to the next returned
      // result and try again.
      std::cerr << "socket() failed: " << strerror(errno) << std::endl;
      // listen_fd_new = -1;
      continue;
    }

    // Configure the socket; we're setting a socket "option."  In
    // particular, we set "SO_REUSEADDR", which tells the TCP stack
    // so make the port we bind to available again as soon as we
    // exit, rather than waiting for a few tens of seconds to recycle it.
    int optval = 1;
    setsockopt(listen_fd_new, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof(optval));

    // Try binding the socket to the address and port number returned
    // by getaddrinfo().
    if (bind(listen_fd_new, rp->ai_addr, rp->ai_addrlen) == 0) {
      // Bind worked! Now we return the ai_family
      sock_family_ = rp->ai_family;
      break;
    }
    // The bind failed.  Close the socket, then loop back around and
    // try the next address/port returned by getaddrinfo().
    close(listen_fd_new);
    listen_fd_new = -1;
  }
  // Free the structure returned by getaddrinfo().
  freeaddrinfo(result);

  // Did we succeed in binding to any addresses?
  if (listen_fd_new == -1) {
    // No.  Quit with failure.
    std::cerr << "Couldn't bind to any addresses." << std::endl;
    return false;
  }
  if (listen(listen_fd_new, SOMAXCONN) != 0) {
    std::cerr << "Failed to mark socket as listening: ";
    std::cerr << strerror(errno) << std::endl;
    close(listen_fd_new);
    return false;
  }

  listen_sock_fd_ = listen_fd_new;
  *listen_fd = listen_fd_new;
  return true;
}

bool ServerSocket::Accept(int* const accepted_fd,
                          std::string* const client_addr,
                          uint16_t* const client_port,
                          std::string* const client_dns_name,
                          std::string* const server_addr,
                          std::string* const server_dns_name) const {
  // Accept a new connection on the listening socket listen_sock_fd_.
  // (Block until a new connection arrives.)  Return the newly accepted
  // socket, as well as information about both ends of the new connection,
  // through the various output parameters.

  // STEP 2:
  int client_fd;
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    struct sockaddr *addr = reinterpret_cast<struct sockaddr *>(&caddr);
    client_fd = accept(listen_sock_fd_, addr, &caddr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR)|| (errno == EWOULDBLOCK))
        continue;
      std::cerr << "Failure on accept: " << strerror(errno) << std::endl;
    }
    *accepted_fd = client_fd;

    // PrintOut function
    if (addr->sa_family == AF_INET) {
      // Print out the IPV4 address and port
      char astring[INET_ADDRSTRLEN];
      struct sockaddr_in *in4 = reinterpret_cast<struct sockaddr_in *>(&addr);
      inet_ntop(AF_INET, &(in4->sin_addr), astring, INET_ADDRSTRLEN);
      *client_addr = std::string(astring);  // Basically the print statements
      *client_port = ntohs(in4->sin_port);  // Transformed print statements
    } else if (addr->sa_family == AF_INET6) {
      // Print out the IPV6 address and port
      char astring[INET6_ADDRSTRLEN];
      struct sockaddr_in6 *in6 = reinterpret_cast<struct sockaddr_in6 *>(addr);
      inet_ntop(AF_INET6, &(in6->sin6_addr), astring, INET6_ADDRSTRLEN);
      *client_addr = std::string(astring);  // Basically the print statements
      *client_port = ntohs(in6->sin6_port);  // Transformed print statements
    } else {
      return false;
    }
    // end of mimicking the PrintOut function

    // Start of PrintReverseDNS function
    char hostname[BUF];  // ought to be big enough
    if (getnameinfo(addr, sizeof(caddr), hostname, BUF, nullptr, 0, 0) != 0) {
      std::cerr << hostname << "[reverse DNS failed]" << std::endl;
    }
    *client_dns_name = std::string(hostname);

    // end of mimicking PrintReverseDNS

    // Start of PrintServerSide function
    char hname[BUF];
    hname[0] = '\0';

    if (sock_family_ == AF_INET) {
      // The server is using an IPv4 address.
      struct sockaddr_in srvr;
      socklen_t srvrlen = sizeof(srvr);
      char addrbuf[INET_ADDRSTRLEN];

      getsockname(client_fd,
                  reinterpret_cast<struct sockaddr *> (&srvr),
                  &srvrlen);
      inet_ntop(AF_INET, &srvr.sin_addr, addrbuf, INET_ADDRSTRLEN);
      *server_addr = std::string(addrbuf);

      // Get the server's dns name, or return it's IP address as
      // a substitute if the dns lookup fails.
      getnameinfo(reinterpret_cast<sockaddr *> (&srvr),
                  srvrlen, hname, BUF, nullptr, 0, 0);

      // setting server dns name instead of print statement
      *server_dns_name = std::string(hname);
    } else {
      // The server is using an IPv6 address.
      struct sockaddr_in6 srvr;
      socklen_t srvrlen = sizeof(srvr);
      char addrbuf[INET6_ADDRSTRLEN];

      getsockname(client_fd,
                  reinterpret_cast<struct sockaddr *> (&srvr),
                  &srvrlen);
      inet_ntop(AF_INET6, &srvr.sin6_addr, addrbuf, INET6_ADDRSTRLEN);

      // setting *server_addr
      *server_addr = std::string(addrbuf);

      // Get the server's dns name, or return it's IP address as
      // a substitute if the dns lookup fails.
      getnameinfo(reinterpret_cast<struct sockaddr *> (&srvr),
                  srvrlen, hname, BUF, nullptr, 0, 0);

      // setting the *server_dns_name
      // instead of a print statement
      *server_dns_name = std::string(hname);
    }  // end of mimicking PrintServerSide function
    break;
  }  // end of while loop

  return true;
}
}  // namespace hw4
