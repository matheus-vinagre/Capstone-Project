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
  vector<vector<string>> cpu = LinuxParser::CpuUtilization();
  vector<Processor> cpu_temp;
  cpu_temp.reserve(LinuxParser::cpuN);
  for(const auto& core : cpu) {
    Processor processor(core);
    cpu_temp.emplace_back(processor);
  }
  cpu_ = cpu_temp;
  cpu_temp.clear();
  return cpu_;
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

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() {
  if (_os && _os->empty()) {
    *_os = LinuxParser::OperatingSystem();
    return *_os;
  }
  return *_os;
}

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return processes_.size(); }

long System::UpTime() { return LinuxParser::UpTime(); }