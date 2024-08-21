#include "processor.h"
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <linux_parser.h>
using std::stoull;
using std::string;
using std::to_string;
using std::vector;

Processor::Processor(const std::vector<std::string>& data)
    : _user(stoull(data[0])),
      _nice(stoull(data[1])),
      _system(stoull(data[2])),
      _idle(stoull(data[3])),
      _iowait(stoull(data[4])),
      _irq(stoull(data[5])),
      _softirq(stoull(data[6])),
      _steal(stoull(data[7])),
      _guest(stoull(data[8])),
      _guest_nice(stoull(data[9])),
      _cpunumber(stoull(data[10]))
      {}
Processor::Processor() : _user(0), _nice(0), _system(0), _idle(0), _iowait(0), _irq(0),
                    _softirq(0), _steal(0), _guest(0), _guest_nice(0), _cpunumber(0){

}


template <typename Type>
  Type Sub(Type a, Type b) { return (a > b ) ? ( a - b ): 0; }


void Processor::Utilization(std::vector<PrevProcessor>* prevProcessor) {
  unsigned long long totaltime =
      _usertime + _nicetime + _systemAlltime + _idleAlltime + _steal + _virtalltime;

  if(prevProcessor->empty()) {
    PrevProcessor prevProc(_cpunumber);
    unsigned long long deltaIdleAlltime = Sub(_idleAlltime, prevProc.LastIdleAlltime());
    unsigned long long deltaTotaltime = Sub(totaltime, prevProc.LastTotaltime());
    prevProc.set_LastTotaltime(totaltime);
    prevProc.set_LastIdleAlltime(_idleAlltime);
    prevProcessor->push_back(prevProc);
    _utilization = (deltaTotaltime - deltaIdleAlltime) / 1000.0;
    return;
  }
  auto it = std::lower_bound(prevProcessor->begin(), prevProcessor->end(),
                _cpunumber, [](const PrevProcessor& processor, unsigned long long cpuNumber) {
                  return processor.GetCpuNumber() < cpuNumber;
                  //lambda function compares the pid of PrevProcess objects to the target pid.
    });
  if (it != prevProcessor->end() && it->GetCpuNumber() == _cpunumber) {
    unsigned long long deltaIdleAlltime = Sub(_idleAlltime, it->LastIdleAlltime());
    unsigned long long deltaTotaltime = Sub(totaltime, it->LastTotaltime());
    it->set_LastTotaltime(totaltime);
    it->set_LastIdleAlltime(_idleAlltime);
    _utilization = (deltaTotaltime - deltaIdleAlltime) / 1000.0;
    return;
  }
  PrevProcessor prevProc(_cpunumber);
  unsigned long long deltaIdleAlltime = Sub(_idleAlltime, prevProc.LastIdleAlltime());
  unsigned long long deltaTotaltime = Sub(totaltime, prevProc.LastTotaltime());
  prevProc.set_LastTotaltime(totaltime);
  prevProc.set_LastIdleAlltime(_idleAlltime);
  prevProcessor->push_back(prevProc);
  _utilization = (deltaTotaltime - deltaIdleAlltime) / 1000.0;
  std::sort(prevProcessor->begin(), prevProcessor->end(), [](const PrevProcessor& a, const PrevProcessor& b) {
        return a.GetCpuNumber() < b.GetCpuNumber();
    });
}

float Processor::GetUtilization() {
  return _utilization;
}

PrevProcessor::PrevProcessor(const unsigned long long cpunumber): _cpunumber(cpunumber) {
}

unsigned long long PrevProcessor::GetCpuNumber() const {
  return _cpunumber;
}

unsigned long long PrevProcessor::LastTotaltime() const {
  return _lastTotaltime;
}

void PrevProcessor::set_LastTotaltime(unsigned long long last_totaltime) {
  _lastTotaltime = last_totaltime;
}

unsigned long long PrevProcessor::LastIdleAlltime() const {
  return _lastIdleAlltime;
}

void PrevProcessor::set_LastIdleAlltime(unsigned long long last_idle_alltime) {
  _lastIdleAlltime = last_idle_alltime;
}

