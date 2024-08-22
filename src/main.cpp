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
  System system; //Initializates the system

  // Captures some system data that needs to create Ncurses display properly
  LinuxParser::ProcStatParsin(&system);
  LinuxParser::OperatingSystem(&system);
  LinuxParser::Kernel(&system);
  LinuxParser::UpTime(&system);
  LinuxParser::Pids(&system);
  LinuxParser::MemoryParse(&system);

  NCursesDisplay::Display(system); // Lauches the display loop
}
