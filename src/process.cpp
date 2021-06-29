#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>


#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid) {
    uptime_ = LinuxParser::UpTime(pid);
    ram_ = LinuxParser::Ram(pid);
    user_ = LinuxParser::User(pid);
    command_= LinuxParser::Command(pid);
   updateCpuUtilization();
}

int Process::Pid()  { return pid_; }

float Process::CpuUtilization() const { 
     
     return cpuUtilitzation_; }

void Process::updateCpuUtilization()  { 
    
        long activeTime =  LinuxParser::ActiveJiffies(pid_);
        long seconds = LinuxParser::UpTime() - uptime_;
        cpuUtilitzation_= (float) activeTime / (float) seconds;
      }

string Process::Command()  { 
    return  command_;}

string Process::Ram() { 
    
    return ram_; }

string Process::User() { 
    
    return user_; }

long int Process::UpTime() { 
    
    return uptime_; }

bool Process::operator<(Process const& a) const { return cpuUtilitzation_< a.CpuUtilization(); }