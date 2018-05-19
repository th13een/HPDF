#include "connector.h"

char* str2ch(std::string s){
  int n = s.length();
  char *ch = (char*)malloc(sizeof(char) * (n+1));
  strcpy(ch,s.c_str());
  return ch;
}

Connector::Connector(std::string type, int bufflen, std::string glblName)
  : type{type}, bufflen{bufflen}, glblName{glblName}, sock_fd{0}
{
  if(type!="SOCK_STREAM" && type!="SOCK_DGRAM")
    throw InvalidConnectorType{};
}

Server::Server(std::string type, int bufflen, std::string glblName)
  : Connector(type, bufflen, glblName), poll_timeout{1}
{}

Client::Client(std::string type, int bufflen) : Connector(type, bufflen, "")
{}

void Connector::setNewGlobalName(std::string name){
  this->glblName = name;
}

int Connector::getfd(){
  return this->sock_fd;
}

std::string Connector::getbuff(){
  return this->buffer;
}

void Connector::setbuff(char* ch){
  (this->buffer).clear();
  std::string str(ch);
  buffer = str;
}

int Connector::getbufflen(){
  return bufflen;
}
  
int Connector::initSockfd(){
  char* ch_glblnm = str2ch(this->glblName);
  
  unlink(ch_glblnm);

  if(this->type == "SOCK_STREAM"){
    this->sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(this->sock_fd == -1) throw InvalidSocketDescriptor{};
    return this->sock_fd;
  }
  else if (this->type == "SOCK_DGRAM") {
    this->sock_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(this->sock_fd == -1) throw InvalidSocketDescriptor{};
    return this->sock_fd;
  }
  else throw InvalidConnectorType{};
}

void Connector::bindSock(){
  memset(&(this->addr), 0, sizeof(this->addr));
  (this->addr).sun_family = AF_UNIX;
  strncpy((this->addr).sun_path, str2ch(this->glblName), sizeof((this->addr).sun_path)-1);
  int retval = bind(this->sock_fd, (struct sockaddr*)&(this->addr), sizeof(this->addr));
  if (retval == -1) throw BindException{};
}

void Server::listenSock(int numconns){
  int retval = listen(this->getfd(), numconns);
  if (retval == -1) throw ListenException{};
}
  
int Server::fullInit_STREAM(int numlistenconns){
  int sd = this->initSockfd();
  this->bindSock();
  this->listenSock(numlistenconns);
  return sd;
}

int Server::acceptSock(int timeout){ // run in while loop after calling listen
  while(1){
    this->poll_timeout = timeout;

    this->pollfds[0].fd = this->getfd();
    this->pollfds[0].events = 0;
    this->pollfds[0].events |= POLLIN;
    std::cout << "Outside ran" << std::endl;

    if(poll(this->pollfds, 1, this->poll_timeout) != 0){
      int newconn = accept(this->getfd(), NULL, NULL);
      if (newconn == -1) throw AcceptException{};
      return newconn;
    }
  }
}

int Server::fullInit_DGRAM(){
  int sd = this->initSockfd();
  this->bindSock();
  return sd;
}

std::string Server::readSock_STREAM(int sd){
  char ch[this->getbufflen()];
  int retval = read(sd, ch, this->getbufflen());
  if (retval == -1) throw ReadException{};
  this->setbuff(ch);
  return this->getbuff();
}

std::string Client::readSock(){
  char ch[this->getbufflen()];
  int retval = read(this->getfd(), ch, this->getbufflen());
  if (retval == -1) throw ReadException{};
  this->setbuff(ch);
  return this->getbuff();
}

std::string Server::readSock_DGRAM(){
  char ch[this->getbufflen()];
  int retval = read(this->getfd(), ch, this->getbufflen());
  if (retval == -1) throw ReadException{};
  this ->setbuff(ch);
  return this->getbuff();
}


void Server::writeSock_STREAM(int sd, std::string data){
  
  int retval = write(sd, str2ch(data), this->getbufflen());
  if(retval == -1) throw WriteException{};
}

void Client::writeSock(std::string data){
  int retval = write(this->getfd(), str2ch(data), this->getbufflen());
  if(retval == -1) throw WriteException{};
}

void Server::writeSock_DGRAM(std::string data){
  int retval = write(this->getfd(), str2ch(data), this->getbufflen());
  if(retval == -1) throw WriteException{};
}

void Client::connect_STREAM(std::string srvr_glblnm){
  (this->addr).sun_family = AF_UNIX;
  strncpy((this->addr).sun_path, str2ch(srvr_glblnm), sizeof((this->addr).sun_path)-1);
  int retval = connect(this->getfd(), (struct sockaddr*)&(this->addr), sizeof(this->addr));
  if(retval == -1) throw ConnectException{};
}

void Client::connect_DGRAM(std::string cli_glblnm, std::string srvr_glblnm){
  memset(&(this->srvr_addr), 0, sizeof(this->srvr_addr));
  (this->srvr_addr).sun_family = AF_UNIX;
  strncpy((this->srvr_addr).sun_path, str2ch(srvr_glblnm), sizeof(srvr_glblnm));
  this->setNewGlobalName(cli_glblnm);
  this->bindSock();
  int retval = connect(this->getfd(), (struct sockaddr*) &(this->srvr_addr), sizeof(this->srvr_addr));
  if(retval == -1) throw ConnectException{};
}

