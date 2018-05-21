/// Connector is a wrapper class that handles (UNIX) socket communication
#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <poll.h>

class Connector {
 public:
  Connector(std::string type, int bufflen, std::string glblName); // SOCK_STREAM or SOCK_DGRAM
  void setNewGlobalName(std::string name);
  struct sockaddr_un addr;
  int getfd();
  std::string getbuff();
  void setbuff(char* ch); 
  int getbufflen();
  int initSockfd();
  void bindSock();
  class InvalidConnectorType{};
  class InvalidSocketDescriptor{};
  class BindException{};
  class ListenException{};
  class AcceptException{};
  class ReadException{};
  class WriteException{};
  class ConnectException{};
    
 private:
  std::string glblName;
  int sock_fd;
  std::string type;
  int bufflen;
  std::string buffer;
};

class Server : public Connector {
 public:
  Server(std::string type, int bufflen, std::string glblName);
  void listenSock(int numconns);
  int fullInit_STREAM(int numlistenconns);
  int acceptSock(int timeout);
  int fullInit_DGRAM();
  std::string readSock_STREAM(int sd);
  std::string readSock_DGRAM();
  void writeSock_STREAM(int sd, std::string data);
  void writeSock_DGRAM(std::string data);
  
 private:
  struct pollfd pollfds[1];
  int poll_timeout;
  
};

class Client : public Connector {
 public:
  Client(std::string type, int bufflen);
  void connect_STREAM(std::string srvr_glblnm);
  void connect_DGRAM(std::string cli_glblnm, std::string srvr_glblnm);
  std::string readSock();
  void writeSock(std::string data);
 private:
  struct sockaddr_un srvr_addr;
};



#endif //CONNECTOR_H
