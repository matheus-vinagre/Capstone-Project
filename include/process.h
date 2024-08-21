#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <vector>
#include <unistd.h>

class PrevProcess;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    Process(int pid, long utime, long stime, long cutime, long cstime,
            long starttime, long sys_uptime, std::string user,
            std::string uid, std::string ram, std::string command);
    long& totaltime();
    void set_totaltime(long last_totaltime);
    long& uptime();
    void set_uptime(long last_uptime);
    int Pid();
    std::string User();
    std::string Command();
    void CpuUtilization(std::vector<PrevProcess>* PrevProcesses);
    float GetCpuUtilization();
    std::string Ram();
    long UpTime();
    bool operator<(Process const& a) const;

private:
    int _pid;
    long _utime, _stime, _cutime, _cstime, _starttime;
    long _sysUptime;
    std::string _user, _uid, _ram, _command;

    long _totaltime = _utime + _stime + _cutime + _cstime;
    long _uptime = _sysUptime - ( _starttime / sysconf(_SC_CLK_TCK));
    float _cpuUsage=0;
};

class PrevProcess {
public:
    explicit PrevProcess(int pid);

    int pid() const;
    void set_pid(int pid);
    long & totaltime();
    void set_totaltime(long totaltime);
    long & uptime();
    void set_uptime(long uptime);
private:
    int _pid;
    long _totaltime = 0;
    long _uptime = 0;
};

#endif