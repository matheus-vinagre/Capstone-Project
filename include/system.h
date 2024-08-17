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

  int* GetRunningProcessRawPtr();
  int* GetTotalProcessRawPtr();
  int* GetCpuNRawPtr();

 private:
  std::vector<Processor> cpu_ ;
  std::vector<Process> processes_;

  std::unique_ptr<std::string> _os = std::make_unique<std::string>();
  std::unique_ptr<std::string> _kernel = std::make_unique<std::string>();
  std::unique_ptr<int> _runningProcess = std::make_unique<int>();
  std::unique_ptr<int> _totalProcess = std::make_unique<int>();
  std::unique_ptr<int> _cpuN = std::make_unique<int>();
};

#endif