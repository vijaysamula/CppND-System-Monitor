#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include <iostream>
using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    
    return     cpu_;
; }

vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for (vector<int>::iterator it= pids.begin(); it!=pids.end();++it) {
        Process process(*it);
        processes_.push_back(process);
    }
    std::sort(processes_.rbegin(),processes_.rend());
    return processes_; }

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { long uptime= LinuxParser::UpTime();
return uptime; }