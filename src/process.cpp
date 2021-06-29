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

// TODO: Return this process's ID
int Process::Pid()  { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
     
     return cpuUtilitzation_; }

void Process::updateCpuUtilization()  { 
    
        long activeTime =  LinuxParser::ActiveJiffies(pid_);
        long seconds = LinuxParser::UpTime() - uptime_;
        cpuUtilitzation_= (float) activeTime / (float) seconds;
      }

// TODO: Return the command that generated this process
string Process::Command()  { 
    return  command_;}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    
    return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    
    return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    
    return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return cpuUtilitzation_< a.CpuUtilization(); }