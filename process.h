/* A process creates a thread that connects to the assembly as a client. A process only starts once it is connected to the assembly. The assembly has a dedicated thread
 * for communicating with a given process. The assembly side of these sockets wait indefinitely for commands. A process may send a command to the assembly, asking to be 
 * connected to another process. The assembly facilitates this by the transfer of socket descriptors. The end result is the requesting process creates another thread in
 * which it acts as server and the requested process also creates another thread and connects to the requesting process as a client. If the process wants to end the
 * connection it simply closes the connection and joins the thread. The idea is that communication between any two processes has both processes create a new thread each
 * to facilitate the communication. This becomes impractical if a process needs to be connected to by many other processes. In this case, shared memory can be used.
 */
#include <iostream>
#include "connector.h"
#include <map.h>

class Process{
 public:
  Process(std::string name);
  void run();
  ~Process();
 private:
  std::string name;
  void conn2asmbl();
  void conn2proc(std::string procnm);
  int main_sd;
  bool exit;
  map<std::string procnm, int sd> proc_sds;
  


};

