#ifndef PROCESS_H
#define PROCESS_H
#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
    explicit Process(int pid);
    long& last_totaltime();
    void set_last_totaltime(long last_totaltime);
    long& last_uptime();
    void set_last_uptime(long last_uptime);
    int Pid();
    std::string User();
    std::string Command();
    float CpuUtilization();
    std::string Ram();
    long UpTime();
    bool operator<(Process const& a) const;

private:
    int pid_;
    long last_totaltime_ = 0;
    long last_uptime_ = 0;
};

#endif