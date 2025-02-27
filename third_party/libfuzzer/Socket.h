//
// Created by sheid on 07.04.21.
//

#ifndef COMPILERRT_SOCKET_H
#define COMPILERRT_SOCKET_H
#include <string>
#include <vector>

using std::string;
typedef struct{
      int mutRep;
      std::string fileContents;
    } dataOut;
typedef union {
    unsigned int Integer;
    unsigned char Byte[4];
  } int_asbytes;
class Socket {
public:
  
  Socket(const char *Path);
  bool read(dataOut *Out);
  bool write(std::vector<std::string> Data, std::vector<bool> IsNonCrashing);
  bool close();
  ~ Socket();
  

private:
  int Sockfd;
  const char* SocketPath;
};

#endif // COMPILERRT_SOCKET_H
