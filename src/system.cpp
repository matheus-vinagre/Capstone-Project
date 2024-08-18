#include "system.h"
#include <linux_parser.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

vector<Processor>& System::Cpu() {
    return *cpu_;
}

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  vector<Process> temp_processes;
  temp_processes.reserve(pids.size());
  for(auto pid : pids) {
    Process process(pid);
    temp_processes.emplace_back(process);
  }
  processes_.swap(temp_processes);
  temp_processes.clear();
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

std::string System::Kernel() {
  if (_kernel && _kernel->empty()) {
    *_kernel = LinuxParser::OperatingSystem(); // Receives an rvalue because of RVO of the compiler
    return *_kernel;
  }
  return *_kernel;
}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() {
  if (_os && _os->empty()) {
    *_os = LinuxParser::OperatingSystem(); // Receives an rvalue because of RVO of the compiler
    return *_os;
  }
  return *_os;
}

int System::RunningProcesses() { return *_runningProcess; }

int System::TotalProcesses() { return processes_.size(); }

long System::UpTime() { return LinuxParser::UpTime(); }

int* System::GetRunningProcessRawPtr() {
  return _runningProcess.get();
}
int* System::GetTotalProcessRawPtr() {
  return _totalProcess.get();
}
int* System::GetCpuNRawPtr() {
  return _cpuN.get();
}
std::vector<Processor>* System::GetCpuVectorRawPtr() {
  return cpu_.get();
}