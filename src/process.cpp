#include "process.h"

#include <linux_parser.h>
#include <unistd.h>
#include <cctype>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

PrevProcess::PrevProcess(int pid): _pid(pid) {}
int PrevProcess::pid() const { return _pid; }
void PrevProcess::set_pid(int pid) { _pid = pid; }
long & PrevProcess::totaltime() { return _totaltime; }
void PrevProcess::set_totaltime(long totaltime) { _totaltime = totaltime; }
long & PrevProcess::uptime() { return _uptime; }
void PrevProcess::set_uptime(long uptime) { _uptime = uptime; }


Process::Process(int pid, long utime, long stime, long cutime, long cstime, long starttime, long sys_uptime,
                 std::string user, std::string uid, std::string ram, std::string command): _pid(pid),
                                                                                           _utime(utime),
                                                                                           _stime(stime),
                                                                                           _cutime(cutime),
                                                                                           _cstime(cstime),
                                                                                           _starttime(starttime),
                                                                                           _sysUptime(sys_uptime),
                                                                                           _user(std::move(user)),
                                                                                           _uid(std::move(uid)),
                                                                                           _ram(std::move(ram)),
                                                                                           _command(std::move(command))
{}

long& Process::totaltime() { return _totaltime; }
void Process::set_totaltime(long last_totaltime) {
  _totaltime = last_totaltime;
}
long& Process::uptime() { return _uptime; }
void Process::set_uptime(long last_uptime) { _uptime = last_uptime; }

double calculateCpu(const long& delta_totaltime, const long& delta_uptime) {
  long clockTicks = sysconf(_SC_CLK_TCK);
  if(delta_uptime == 0) {
    return 0;
  }
  double dd_delta_totaltime = delta_totaltime * 10.0;
  double cpuPercent = ((dd_delta_totaltime  / clockTicks) / delta_uptime);
  return cpuPercent;
}
vector<long> Prev(int pid, long totaltime, long uptime, std::vector<PrevProcess>* prevProcesses) {
  // if theres no process saved inside the vector of Previus processes, it adds the first
  if(prevProcesses->empty()) {
    PrevProcess prevProc(pid);
    prevProc.set_totaltime(totaltime);
    prevProc.set_uptime(uptime);
    prevProcesses->push_back(prevProc);
    vector<long> prev_totaltime_and_uptime({0 ,0});
    return prev_totaltime_and_uptime;
  }
  // try to find the previus process with the pid number, if found,
  // returns the values of totaltime and uptime and atualizates it.
  auto it = std::lower_bound(prevProcesses->begin(), prevProcesses->end(),
                pid, [](const PrevProcess& process, int pid) {
                  return process.pid() < pid;
                  //lambda function compares the pid of PrevProcess objects to the target pid.
    });
  if (it != prevProcesses->end() && it->pid() == pid) {
    long last_totaltime = it->totaltime();
    long last_uptime = it->uptime();
    it->set_totaltime(totaltime);
    it->set_uptime(uptime);
    vector<long> prevProcess({last_totaltime, last_uptime});
    return prevProcess;
  }
  // if isn't empty and dosen't find the exact pid number, create and push it inside the vector
  PrevProcess prevProc(pid);
  prevProc.set_totaltime(totaltime);
  prevProc.set_uptime(uptime);
  prevProcesses->push_back(prevProc);
  std::sort(prevProcesses->begin(), prevProcesses->end(), [](const PrevProcess& a, const PrevProcess& b) {
        return a.pid() < b.pid();
    });
  vector<long> prev_totaltime_and_uptime({0 ,0});
  return prev_totaltime_and_uptime;
}

template <typename Type>
Type Sub(Type a, Type b) { return (a > b) ? (a - b) : 0; }

float Process::CpuUtilization(std::vector<PrevProcess>* prevProcesses) {
  vector<long> prevProcess = Prev(_pid,_totaltime, _uptime, prevProcesses );
  long d_totaltime = Sub(_totaltime, prevProcess[0]);
  long d_uptime = Sub(_uptime,prevProcess[1]);
  double cpuUsage = calculateCpu(d_totaltime, d_uptime);
  return cpuUsage;

}

int Process::Pid() { return _pid; }

string Process::Command() { return _command; }

string Process::Ram() {return _ram;}

string Process::User() { return _user; }

long int Process::UpTime() {
  return _uptime;
}

// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& other) const {
  float a = std::stof(_ram);
  float b = std::stof(other._ram);
  return a > b;
}





