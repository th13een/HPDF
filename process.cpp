#include "process.h"

Process::Process(std::string name, std::string assemblyname) : name{name}, asmblnm{assemblyname}{
  this->conn2asmbl();
}
