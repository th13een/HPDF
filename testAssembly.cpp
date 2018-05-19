#include <iostream>
#include "connector.h"
#include <map>
#include <thread>

#define BUFFLEN 1000
#define MAIN_SOCK_NAME "AssemblySock"
#define NUMLISTEN 5
#define TIMEOUT 5000

std::map<std::string, int> dataSockList;
std::map<std::string, std::thread> threadList;
bool shutdownRequested = false;
int serverSock;
int dataSock;

int main(){
  Server mainsock = Server("SOCK_STREAM",BUFFLEN, MAIN_SOCK_NAME);
  mainsock.fullInit_STREAM(NUMLISTEN);

  while(!shutdownRequested){
    dataSock = mainsock.acceptSock(TIMEOUT);
    std::string name = mainsock.readSock_STREAM(dataSock);
    dataSockList[name] = dataSock;
    std::cout << "Name read " << name << std::endl;
  }
}
