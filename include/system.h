#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include "memory.h"
#include "process.h"
#include "processor.h"

class System {
    public:
        //Setters
        void SetRunningProcess(int runningProcess) const;
        void SetTotalProcesses(int totalProcesses) const;
        void SetOs(const std::string &os) const;
        void SetKernel(const std::string &kernel) const;
        void SetCpuNumber(int cpuNumber) const;
        void SetUptime(long uptime) const;
        void SetMemory(const Memory &memory) const;

        //Getters
        std::vector<Processor>& Cpu();
        std::vector<Process>& Processes();
        std::string Kernel();
        std::string OperatingSystem();
        int TotalProcesses();
        int RunningProcesses();
        float MemoryUtilization();
        long UpTime();

        //RawPointer getters
        Memory* GetMemory() const;
        std::vector<Processor>* GetCpuVectorRawPtr() const;
        std::vector<Process>* GetProcessVectorRawPrt() const;
        std::vector<PrevProcessor>* GetPrevCpuVectorRawPtr() const;
        std::vector<PrevProcess>* GetPrevProcessVectorRawPrt() const;

        void UpdateSystemMutex();
    private:
        std::mutex _systemMutex;

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