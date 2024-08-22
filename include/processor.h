#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class PrevProcessor;

class Processor {
public:
    //Constructors
    explicit Processor(const std::vector<std::string>& data);
    Processor();

    //Calculates the cpu utilization for a cpu
    void Utilization(std::vector<PrevProcessor>* prevProcessor);

    //Getter
    float GetUtilization();

private:
    unsigned long long _user, _nice, _system, _idle, _iowait, _irq,
                    _softirq, _steal, _guest, _guest_nice, _cpunumber;
    unsigned long long _usertime = _user - _guest;
    unsigned long long _nicetime = _nice - _guest_nice;
    unsigned long long _idleAlltime = _idle + _iowait;
    unsigned long long _systemAlltime = _system + _irq + _softirq;
    unsigned long long _virtalltime = _guest + _guest_nice;

    float _utilization = 0;

};
class PrevProcessor {
public:
    //Constructor
    explicit PrevProcessor(unsigned long long cpunumber);

    //Getters
    unsigned long long GetCpuNumber() const;
    unsigned long long LastTotaltime() const;
    unsigned long long LastIdleAlltime() const;

    //Setters
    void set_LastTotaltime(unsigned long long last_totaltime);
    void set_LastIdleAlltime(unsigned long long last_idle_alltime);

private:
    unsigned long long _cpunumber;
    unsigned long long _lastTotaltime = 0, _lastIdleAlltime = 0;

};
#endif
