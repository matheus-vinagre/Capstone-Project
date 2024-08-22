#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <vector>
#include <unistd.h>

class PrevProcess; //foward declaration

class Process {
public:
    //Constructor
    Process(int pid, long utime, long stime, long cutime, long cstime,
            long starttime, long sys_uptime, std::string user,
            std::string uid, std::string ram, std::string command);

    //Getters and Setters
    long& totaltime();
    void set_totaltime(long last_totaltime);
    long& uptime();
    void set_uptime(long last_uptime);
    int Pid();
    std::string User();
    std::string Command();
    float GetCpuUtilization();
    std::string Ram();
    long UpTime();

    //Calculate the cpu utilization for a process
    void CpuUtilization(std::vector<PrevProcess>* PrevProcesses);

    //Used to sort the vector by the ram usage
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
    //constructor
    explicit PrevProcess(int pid);

    //Getters and Setters
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