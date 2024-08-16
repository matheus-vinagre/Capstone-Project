#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  std::vector<Processor>& Cpu();      // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp

  // x TODO: Define any necessary private members
 private:
  std::vector<Processor> cpu_ ;
  std::vector<Process> processes_;

  std::unique_ptr<std::string> _os = std::make_unique<std::string>();
};

#endif