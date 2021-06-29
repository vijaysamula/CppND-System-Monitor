#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include<linux_parser.h>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization()const;      
  void updateCpuUtilization();            
  std::string Ram();                      
  long int UpTime();                       
  bool operator<(Process const& a) const ;  

  // TODO: Declare any necessary private members
 private:
    int pid_;
    std::string command_;
    std::string ram_;
    long uptime_;
    std::string user_;
    float cpuUtilitzation_;

    long activeTime_;
    long totalTime_;
   
};

#endif