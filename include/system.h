#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <string>
#include <vector>

#include "memory.h"
#include "process.h"
#include "processor.h"

class System {
    public:
        std::vector<Processor>& Cpu();
        std::vector<Process>& Processes();
        float MemoryUtilization();
        long UpTime();
        int TotalProcesses();
        int RunningProcesses();
        std::string Kernel();
        std::string OperatingSystem();

        int* GetRunningProcessRawPtr();
        int* GetTotalProcessRawPtr();
        int* GetCpuNRawPtr();
        long* GetUptimeRawPtr();
        std::string* GetOsRawPtr();
        std::string* GetKernelRawPtr();
        Memory* GetMemoryRawPtr();
        std::vector<Processor>* GetCpuVectorRawPtr();
        std::vector<Process>* GetProcessVectorRawPrt();
        std::vector<PrevProcessor>* GetPrevCpuVectorRawPtr();
        std::vector<PrevProcess>* GetPrevProcessVectorRawPrt();
    private:

        std::unique_ptr<std::vector<Processor>> _cpu = std::make_unique<std::vector<Processor>>();
        std::unique_ptr<std::vector<PrevProcessor>> _prevCpu = std::make_unique<std::vector<PrevProcessor>>();
        std::unique_ptr<std::vector<Process>> _processes = std::make_unique<std::vector<Process>>();
        std::unique_ptr<std::vector<PrevProcess>> _prevProcesses = std::make_unique<std::vector<PrevProcess>>();
        std::unique_ptr<Memory> _memory = std::make_unique<Memory>();

        std::unique_ptr<std::string> _os = std::make_unique<std::string>();
        std::unique_ptr<std::string> _kernel = std::make_unique<std::string>();
        std::unique_ptr<int> _runningProcess = std::make_unique<int>();
        std::unique_ptr<int> _totalProcess = std::make_unique<int>();
        std::unique_ptr<int> _cpuN = std::make_unique<int>();
        std::unique_ptr<long> _upTime = std::make_unique<long>();
};

#endif