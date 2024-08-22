#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include "linux_parser.h"
#include "format.h"
#include "ncurses_display.h"
#include "system.h"
#include <thread>

using std::string;
using std::to_string;

// Function to process data in separate threds
void CpuUtilizationThread(System& system) {
  for (unsigned long long i = 0; i < system.Cpu().size(); i++) {
    system.Cpu()[i].Utilization(system.GetPrevCpuVectorRawPtr());
  }
}
// Function to process data in separate threds
void ProcessCpuUtilizationThread(System& system) {
  for (size_t i = 0; i < system.Processes().size(); i++) {
    system.Processes()[i].CpuUtilization(system.GetPrevProcessVectorRawPrt());
  }
}

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent) {
  std::string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());

// Changed the original code for multiples cpu cores
  size_t cpuCount = system.Cpu().size();
  for (size_t i = 0; i < cpuCount; ++i) {
    mvwprintw(window, ++row, 2, ("CPU " + std::to_string(i) + ": ").c_str());
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "");
    wprintw(window, ProgressBar(system.Cpu()[i].GetUtilization()).c_str());
    wattroff(window, COLOR_PAIR(1));
  }

  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization()).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,
            ("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(
      window, ++row, 2,
  ("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  mvwprintw(window, ++row, 2,
            ("Up Time: " + Format::ElapsedTime(system.UpTime())).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW* window, int n) {
  int row{0};
  int const pid_column{2};
  int const user_column{9};
  int const cpu_column{18};
  int const ram_column{26};
  int const time_column{35};
  int const command_column{46};
  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, pid_column, "PID");
  mvwprintw(window, row, user_column, "USER");
  mvwprintw(window, row, cpu_column, "CPU[%%]");
  mvwprintw(window, row, ram_column, "RAM[MB]");
  mvwprintw(window, row, time_column, "TIME+");
  mvwprintw(window, row, command_column, "COMMAND");
  wattroff(window, COLOR_PAIR(2));
  int const num_processes = int(processes.size()) > n ? n : processes.size();
  for (int i = 0; i < num_processes; ++i) {
    mvwprintw(window, ++row, pid_column, to_string(processes[i].Pid()).c_str());
    mvwprintw(window, row, user_column, processes[i].User().c_str());
    float cpu = processes[i].GetCpuUtilization(); // *100 original
    mvwprintw(window, row, cpu_column, to_string(cpu).substr(0, 4).c_str());
    mvwprintw(window, row, ram_column, processes[i].Ram().c_str());
    mvwprintw(window, row, time_column,
              Format::ElapsedTime(processes[i].UpTime()).c_str());
    mvwprintw(window, row, command_column,
              processes[i].Command().substr(0, getmaxx(window) - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  int cpuCount = system.Cpu().size();
  WINDOW* system_window = newwin(8+cpuCount, x_max - 1, 0, 0);
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, getmaxy(system_window) + 1, 0);

  std::vector<std::thread> threads; // creates a vector for the threads
  while (1) {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);

    //Iniciates threads for the collection of data
    threads.emplace_back([&]() { LinuxParser::ProcStatParsin(&system); });
    threads.emplace_back([&]() { LinuxParser::UpTime(&system); });
    threads.emplace_back([&]() { LinuxParser::Pids(&system); });
    threads.emplace_back([&]() { LinuxParser::MemoryParse(&system); });

    //Join those threads
    for (auto& thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }

    // Create threads for CPU utilization and process CPU utilization
    std::thread cpuThread(CpuUtilizationThread, std::ref(system));
    std::thread processThread(ProcessCpuUtilizationThread, std::ref(system));

    // Join the threads (wait for them to finish)
    if (cpuThread.joinable()) {
      cpuThread.join();
    }
    if (processThread.joinable()) {
      processThread.join();
    }
    //Calculate system memory utilization
    system.GetMemory()->MemoryUtilization();

    DisplaySystem(system, system_window); //Display information of the system
    DisplayProcesses(system.Processes(), process_window, n);//Display information of the processes
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    threads.clear(); //clear the threads vector for next interation
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  endwin();
}
