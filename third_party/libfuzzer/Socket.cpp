//
// Created by sheid on 07.04.21.
//

#include "Socket.h"
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

using std::string;

Socket::Socket(const char *SockPath) {
  // sending arbitrary long msgs: https://stackoverflow.com/a/2862176/13310191

  // create socket
  sockaddr_un Address;
  Sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  SocketPath = SockPath;
  if (Sockfd == -1) {
    std::cerr << "could not create unix socket" << std::endl;
    exit(EXIT_FAILURE);
  }

  Address.sun_family = AF_UNIX;
  strcpy(Address.sun_path, SocketPath);

  // connect to socket
    if (connect(Sockfd, (sockaddr *)&Address, sizeof(Address)) < 0){
        std::cerr << "could not connect to: '" << SocketPath << "'" << std::endl;
        ::close(Sockfd);
        exit(EXIT_FAILURE);
    }
  std::cout << "Client established connection";
}

bool Socket::read(dataOut *Out) {
  int_asbytes Len;
  int_asbytes MutRep;

  // recieve number of mutation repititions
  ::recv(Sockfd, MutRep.Byte, 4, 0);
  Out->mutRep = MutRep.Integer;

  // recieve length of the upcomming msg
  ::recv(Sockfd, Len.Byte, 4, 0);
  if (Len.Integer == 0) {
    return false;
  }
  int N;
  char RetValue[Len.Integer];
  // now reading file contents
  N = ::read(Sockfd, RetValue, Len.Integer);
  if (N < 0) {
    std::cout << "socket read failed" << std::endl;
    exit(EXIT_FAILURE);
  }

  // explicitly provide the length of number of bytes to process in the string constructor, otherwise it will stop after the first null
  // https://stackoverflow.com/a/4691703
  Out->fileContents = string(RetValue, RetValue + Len.Integer);
  return true;
}

bool Socket::write(std::vector<std::string> Data, std::vector<bool> IsNonCrashing) {
  int_asbytes Len;
  // send number of inputs
  Len.Integer = Data.size();
  if (::write(Sockfd, Len.Byte, 4) == -1) {
    std::cout << "Error" << std::endl;
    exit(EXIT_FAILURE);
  }
   for(int i = 0; i < Data.size(); i++){
    auto Dat = Data[i];
    auto Succ = IsNonCrashing[i];

    // send length of the cov vector and the vector itself
    Len.Integer = Dat.size();
    if (::write(Sockfd, Len.Byte, 4) == -1) {
      std::cout << "Error" << std::endl;
      exit(EXIT_FAILURE);
    }

    if (::write(Sockfd, Dat.c_str(), Dat.size()) == -1) {
      std::cout << "Error" << std::endl;
      exit(EXIT_FAILURE);
    }

    // Send information whether the file has caused the PuT to crash
    if (::write(Sockfd, &Succ, 1) == -1) {
      std::cout << "Error" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return true;
}

bool Socket::close() {
  ::close(Sockfd);
  unlink(SocketPath);
  return true;
}

Socket::~Socket() { unlink(SocketPath); }
