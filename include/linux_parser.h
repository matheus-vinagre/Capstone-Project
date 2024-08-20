#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <processor.h>
#include <process.h>
#include <system.h>
#include <regex>
#include <string>
#include <experimental/filesystem>
#include <boost/filesystem.hpp>


namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

//Proc/stat parsing
void ProcStatParsin(System* system);

// System
void MemoryParse(Memory* memory);
void UpTime(long* upTime);
void Pids(std::vector<Process>* processes);



extern int totalProcesses;
extern int runningProcesses;
extern std::vector<PrevProcessor> prevProcessor;
extern std::vector<std::vector<std::string>> cpuUtilization;
extern int cpuN;
extern std::vector<Process> prevProcesses;

int CpuN();

int TotalProcesses();
int RunningProcesses();
void OperatingSystem(std::string* os);
void Kernel(std::string* kernel);

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_ = 1,
  kSystem_ = 2,
  kIdle_ = 3,
  kIOwait_ = 4,
  kIRQ_ = 5,
  kSoftIRQ_ = 6,
  kSteal_ = 7,
  kGuest_ = 8,
  kGuestNice_ = 9,
  kCpuNumber = 10
};

std::vector<std::vector<std::string>> CpuUtilization();

// Processes
namespace fs = boost::filesystem;

long SysUpTime();

long Jiffies();
long ActiveJiffies();
std::vector<long> ActiveJiffies(const std::string& spid);
long IdleJiffies();
std::string Command(int pid);
std::string Ram(const std::string& spid);
std::string Uid(const std::string& spid);
std::string User(const std::string& uid);
//long int UpTime(const std::string& spid);
void InitializePrevProcessor(int N);
};  // namespace LinuxParser

#endif