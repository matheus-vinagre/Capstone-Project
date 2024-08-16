#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  std::vector<Processor>& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  std::vector<Processor> cpu_ ;
  std::vector<Process> processes_;

  std::unique_ptr<std::string> _os = std::make_unique<std::string>();
};

#endif