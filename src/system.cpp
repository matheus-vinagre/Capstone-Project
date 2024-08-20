#include "system.h"
#include <linux_parser.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"
#include <memory.h>

using std::set;
using std::size_t;
using std::string;
using std::vector;

vector<Processor>& System::Cpu() {
    return *_cpu;
}

vector<Process>& System::Processes() {
  std::sort(_processes->begin(), _processes->end());
  return *_processes;
}

std::string System::Kernel() { return *_kernel; }

float System::MemoryUtilization() {
  _memory->MemoryUtilization();
  return _memory->mem_percent_util();
}

std::string System::OperatingSystem() { return *_os; }

int System::RunningProcesses() { return *_runningProcess; }

int System::TotalProcesses() { return _processes->size(); }

long System::UpTime() { return *_upTime; }

int* System::GetRunningProcessRawPtr() {
  return _runningProcess.get();
}
int* System::GetTotalProcessRawPtr() {
  return _totalProcess.get();
}
int* System::GetCpuNRawPtr() {
  return _cpuN.get();
}
long* System::GetUptimeRawPtr() {
  return _upTime.get();
}
Memory* System::GetMemoryRawPtr() {
  return _memory.get();
}
std::string* System::GetOsRawPtr() {
  return _os.get();
}
std::string* System::GetKernelRawPtr() {
  return _kernel.get();
}
std::vector<Processor>* System::GetCpuVectorRawPtr() {
  return _cpu.get();
}
std::vector<Process>* System::GetProcessVectorRawPrt() {
  return _processes.get();
}
std::vector<PrevProcess>* System::GetPrevProcessVectorRawPrt() {
  return _prevProcesses.get();
}