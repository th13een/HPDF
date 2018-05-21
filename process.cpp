#include <iostream>
#include "process.h"

Process::Process(std::string name, std::string assemblyname) : sock_asm("SOCK_STREAM", MAINBUFFLEN), name{name}, asmblnm{assemblyname}{
  this->conn2asmbl();
}
void Process::conn2asmbl(){
  ((Connector)this->sock_asm).initSockfd(); // create socket descriptor and store in sock_asm.sock_fd
  std::cout << "Assembly Name " << this->asmblnm << std::endl;
  (this->sock_asm).connect_STREAM(this->asmblnm); // attempt connection to assembly 
  (this->sock_asm).writeSock(this->name); // register with assembly on successful connection
}
