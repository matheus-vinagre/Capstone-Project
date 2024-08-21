#include "linux_parser.h"
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <experimental/filesystem>
#include <boost/filesystem.hpp>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <system.h>
#include <vector>
#include <processor.h>
#include <memory.h>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Collect system and cpu data from proc/stat
void LinuxParser::ProcStatParsin(System* system) {
  vector<string> cpuData(11);
  string s_totalProcesses, s_runningProcess;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  vector<Processor> cpu_temp;
  string line;
  int cpuCount = 0;
  while (std::getline(filestream, line)) {
    if (line.substr(0, 3) == "cpu") {
      cpuCount++;
      std::istringstream linestream(line);
      linestream.ignore(256, ' '); // Skip the "cpu" part
      linestream >> cpuData[kUser_] >> cpuData[kNice_] >> cpuData[kSystem_]
                 >> cpuData[kIdle_] >> cpuData[kIOwait_] >> cpuData[kIRQ_]
                 >> cpuData[kSoftIRQ_] >> cpuData[kSteal_] >> cpuData[kGuest_]
                 >> cpuData[kGuestNice_];
      cpuData[kCpuNumber] = to_string( cpuCount - 1 );
      Processor processor(cpuData);
      cpu_temp.emplace_back(processor);
    }
    if (line.substr(0, 9) == "processes") {
      std::istringstream linestream(line);
      linestream.ignore(256, ' ');
      linestream >> s_totalProcesses;
      *system->GetTotalProcessRawPtr() = std::stoi(s_totalProcesses);
    }
    if (line.substr(0, 13) == "procs_running") {
      std::istringstream linestream(line);
      linestream.ignore(256, ' ');
      linestream >> s_runningProcess;
      *system->GetRunningProcessRawPtr() = std::stoi(s_runningProcess);
    }
  }
  *system->GetCpuNRawPtr() = cpuCount;
  system->GetCpuVectorRawPtr()->swap(cpu_temp);
}

//Collect the OS name
void LinuxParser::OperatingSystem(std::string* os) {
  string os_temp;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    string key;
    string line;
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> os_temp) {
        if (key == "PRETTY_NAME") {
          std::replace(os_temp.begin(), os_temp.end(), '_', ' ');
          *os = os_temp;
        }
      }
    }
  }
}

//Collect kernel version
void LinuxParser::Kernel(std::string* kernel) {
  string kernel_temp;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    string line;
    string version;
    string os;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel_temp;
  }
  *kernel = kernel_temp;
}

//auxiliary function to verify if its only numbers in a string
bool is_digits(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

// Function to create the vector of processes
namespace fs = boost::filesystem;
void LinuxParser::Pids(std::vector<Process>* processes) {
    std::vector<Process> temp_processes;
    const fs::path dirpath = "/proc"; // Update with your directory path
    // Iterate over the directory entries
    for (fs::directory_iterator it(dirpath); it != fs::directory_iterator(); ++it) {
        if (fs::is_directory(it->path())) { // Use fs::is_directory to check if it's a directory
            std::string dirName = it->path().filename().string();
            if (is_digits(dirName)) {
                int pid = std::stoi(dirName);
                vector<long> pidData = ActiveJiffies(dirName);
                std::string pidCommand = Command(pid);
                std::string pidRam = Ram(dirName);
                std::string pidUid = Uid(dirName);
                std::string pidUser = User(pidUid);
                long sysUptime = SysUpTime();
                Process process(pid,pidData[0], pidData[1], pidData[2], pidData[3],
                                pidData[4], sysUptime, pidUser, pidUid, pidRam, pidCommand);
                temp_processes.emplace_back(process);
            }
        }
    }
  processes->swap(temp_processes);
  temp_processes.clear();
}

//Collect pid cpu data
vector<long> LinuxParser::ActiveJiffies(const std::string& spid) {
  long utime, stime, cutime, cstime, starttime;
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kStatFilename);
  string line;
  string data;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    for(int i=0; i<22;++i ) {
      linestream >> data;
      switch (i) {
        case 13:
          utime = std::stol(data);
        break;
        case 14:
          stime = std::stol(data);
        break;
        case 15:
          cutime = std::stol(data);
        break;
        case 16:
          cstime = std::stol(data);
        break;
        case 21:
          starttime = std::stol(data);
        break;
        default:
          break;
      }
    }
  }
  vector<long> datas({utime, stime, cutime, cstime, starttime});
  return datas;
}

//Collect command line for a specific Pid
string LinuxParser::Command(int pid) {
  std::string spid = to_string(pid);
  std::ifstream filestream(LinuxParser::kProcDirectory + spid + LinuxParser::kCmdlineFilename);
  string line;

  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      // Successfully read the line
      if (line.empty()) {
        std::cerr << "Warning: Command line for PID " << spid << " is empty." << std::endl;
      }
    } else {
     // std::cerr << "Error: Could not read the command line for PID " << spid << std::endl;
    }
  } else {
    std::cerr << "Error: Could not open file for PID " << spid << std::endl;
  }

  return line;
}

//Collect ram data for a specific Pid
string LinuxParser::Ram(const std::string& spid) {
  string ram, kb;
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kStatusFilename);
  string line;
  while (std::getline(filestream, line)) {
    if (line.substr(0, 6) == "VmRSS:") { // Used VmRSS instead of VmSize
      std::istringstream linestream(line.substr(7));
      linestream >> ram >> kb;
      long double kram = std::stoull(ram);
      long double mram = kram /(1024);
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(2) << mram;
      string sram = oss.str();
      return (sram);
    }
  }
  return  "0";
}

//Collect User id for a specific Pid
string LinuxParser::Uid(const std::string& spid) {
  string uid, uid2 ,uid3, uid4;
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kStatusFilename);
  string line;
    while (std::getline(filestream, line)) {
      if (line.substr(0, 4) == "Uid:") {
        std::istringstream linestream(line.substr(5));
        linestream >> uid >> uid2 >> uid3 >> uid4;
    }
  }
  return uid;
}

//Collect User name for a specific Pid
string LinuxParser::User(const string& uid) {
  string user, psswd, key, gid, gecos, dir, shell;
  std::ifstream filestream(kPasswordPath);
  string line;
  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while (linestream >> user >> psswd >> key >> gid >> gecos >> dir >> shell) {
      if (key == uid) {
        return user;
      }
    }
  }
  return "error";
}

bool operator==(const string& lhs, long rhs);

//Collect system uptime
void LinuxParser::UpTime(long* upTime) {
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kUptimeFilename);
  long upTime_temp;
  if (filestream.is_open()) {
    string line, s_upTime, s_idleTime;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> s_upTime >> s_idleTime;
    upTime_temp = std::stol(s_upTime);
  }
  *upTime = upTime_temp;
}

//Collect system uptime
long LinuxParser::SysUpTime() {
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kUptimeFilename);
  long upTime_temp;
  if (filestream.is_open()) {
    string line, s_upTime, s_idleTime;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> s_upTime >> s_idleTime;
    upTime_temp = std::stol(s_upTime);
  }
  return upTime_temp;
}

//Collect memory data
void LinuxParser::MemoryParse(Memory* memory) {
  enum class Choice{A, T, F, B, C, H, R};
  std::map<string, Choice> stringToEnumMap
      = { {"bugKill", Choice::A},
          {"MemTotal:", Choice::T},
          {"MemFree:", Choice::F},
          {"Buffers:", Choice::B},
          {"Cached:", Choice::C},
          {"Shmem:", Choice::H},
          {"SReclaimable:", Choice::R}};
  std::ifstream filestream(LinuxParser::kProcDirectory +
                         LinuxParser::kMeminfoFilename);
  Memory memory_temp;
  if (filestream.is_open()) {
    Choice choice;
    string line;
    string kB;
    string sValue;
    string key;
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> sValue >> kB) {
        choice = stringToEnumMap[key];
        switch (choice) {
          case Choice::T:
            memory_temp.set_mem_total(stoll(sValue));
          break;
          case Choice::F:
            memory_temp.set_mem_free(stoll(sValue));
          break;
          case Choice::B:
            memory_temp.set_mem_buffer(stoll(sValue));
          break;
          case Choice::C:
            memory_temp.set_mem_cached(stoll(sValue));
          break;
          case Choice::H:
            memory_temp.set_s_hmem(stoll(sValue));
          break;
          case Choice::R:
            memory_temp.set_s_reclaimable(stoll(sValue));
          break;
          default:
            break;
        }
      }
    }
  }
  *memory = memory_temp;
}