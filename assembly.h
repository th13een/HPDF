// -*- mode : c++ -*-
/* Written by th13teen - 15/05/2018
 * The "assembly" process runs here. It receives connections from processes,
 * responds to their requests, handles permission to resources and acts as 
 * central process of the framework.
 */
#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <iostream>
#include <map>
#include <thread>
#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>

class Assembly{
public:
  Assembly(std::size_t bufferSize,std::string fdName, int pollTimeout);
  void start();
  void getProcess(std::string name);
  void getResource(std::string name);
  void shutdown();
  
  
private:
  std::size_t bufferSize;
  std::string fdName;
  int timeout;
  std::map<std::string,std::thread> threads;
  std::map<std::string,int> sockets;
  bool toShutdown;


#endif
