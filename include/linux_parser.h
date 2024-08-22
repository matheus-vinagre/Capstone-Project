#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <processor.h>
#include <process.h>
#include <system.h>
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

// System parsing
void ProcStatParsin(System* system); // Collect system and cpu data from proc/stat
void MemoryParse(System* system); //Collect memory data
void UpTime(System* system); //Collect system uptime
void Pids(System* system); // Function to create the vector processes
void OperatingSystem(System* system); //Collect the OS name
void Kernel(System* system); //Collect kernel version

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

// Processes
long SysUpTime(); //Collect system uptime
std::vector<long> ActiveJiffies(const std::string& spid); //Collect pid cpu data
std::string Command(int pid); //Collect command line for a specific Pid
std::string Ram(const std::string& spid); //Collect ram data for a specific Pid
std::string Uid(const std::string& spid); //Collect User id for a specific Pid
std::string User(const std::string& uid); //Collect User name for a specific Pid

};  // namespace LinuxParser

#endif