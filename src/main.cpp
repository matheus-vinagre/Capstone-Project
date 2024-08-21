#include <linux_parser.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <iostream>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;

  //Collect Data thread
  LinuxParser::ProcStatParsin(&system);
  LinuxParser::OperatingSystem(system.GetOsRawPtr());
  LinuxParser::Kernel(system.GetKernelRawPtr());
  LinuxParser::UpTime(system.GetUptimeRawPtr());
  LinuxParser::Pids(system.GetProcessVectorRawPrt());
  LinuxParser::MemoryParse(system.GetMemoryRawPtr());

  /*Process Data thread
  for(unsigned long long i = 0; i < system.Cpu().size();i++ ) {
    system.Cpu()[i].Utilization(system.GetPrevCpuVectorRawPtr());
  }
  for(size_t i = 0; i<system.Processes().size(); i++) {
    system.Processes()[i].CpuUtilization(system.GetPrevProcessVectorRawPrt());
  }
  system.GetMemoryRawPtr()->MemoryUtilization();
  */

  // Display Data thread
  NCursesDisplay::Display(system);
}
