#include "system.h"
#include <linux_parser.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"
#include <memory.h>
#include <mutex>

using std::set;
using std::size_t;
using std::string;
using std::vector;

void System::UpdateSystemMutex() {
  std::lock_guard<std::mutex> lock(_systemMutex);
  // Update ProcStat data
}

vector<Processor>& System::Cpu() {
    return *_cpu;
}

vector<Process>& System::Processes() {
  std::sort(_processes->begin(), _processes->end());
  return *_processes;
}

std::string System::Kernel() { return *_kernel; }

float System::MemoryUtilization() {
  return _memory->mem_percent_util();
}

std::string System::OperatingSystem() { return *_os; }

int System::RunningProcesses() { return *_runningProcess; }

int System::TotalProcesses() { return _processes->size(); }

long System::UpTime() { return *_upTime; }

void System::SetRunningProcess(const int runningProcess) const {
  *_runningProcess = runningProcess;
}

void System::SetTotalProcesses(const int totalProcesses) const {
  *_totalProcess = totalProcesses;
}

void System::SetCpuNumber(const int cpuNumber) const {
  *_cpuN = cpuNumber;
}
void System::SetUptime(const long uptime) const {
  *_upTime = uptime;
}

void System::SetMemory(const Memory &memory) const {
  *_memory = memory;
}
Memory* System::GetMemory() const {
  return _memory.get();
}

void System::SetOs(const std::string &os) const {
  *_os = os;
}

void System::SetKernel(const std::string &kernel) const {
  *_kernel = kernel;
}

std::vector<Processor>* System::GetCpuVectorRawPtr() const {
  return _cpu.get();
}

std::vector<Process>* System::GetProcessVectorRawPrt() const {
  return _processes.get();
}

std::vector<PrevProcessor>* System::GetPrevCpuVectorRawPtr() const {
  return _prevCpu.get();
}

std::vector<PrevProcess>* System::GetPrevProcessVectorRawPrt() const {
  return _prevProcesses.get();
}
