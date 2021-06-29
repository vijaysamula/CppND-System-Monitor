#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "linux_parser.h"
#include <stdlib.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel.substr();
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float memTotal,memFree,buffers,cached;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
 
  if (filestream.is_open()) {
      for (int i =0 ;i<5;++i){
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while(linestream >> key >> value) {
        //std::cout<<key << " "<< value<<"\n";
        if (key == "MemTotal:") memTotal= stof(value);
        else if(key == "MemFree:") memFree= stof(value);
        else if(key == "Buffers:") buffers= stof(value);
        else if(key == "Cached:") cached= stof(value);
        }
      }
    
  }
  filestream.close();

   return (memTotal-memFree-buffers-cached)/memTotal; 
  }

long LinuxParser::UpTime() { 
  long seconds =0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
   
    std::istringstream linestream(line);
    linestream >> seconds  ;
    return seconds; 
  }
  stream.close();
  return seconds; 
  }

long LinuxParser::Jiffies() { return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); }


long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string jiffy;
  long totalJiffy{0}, uTime{0},kTime{0},cTime{0},cK{0};
  vector<string> jiffies;
  
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if (stream.is_open()){
    std::getline(stream , line);
    std::istringstream linestream(line);
    while(linestream>>jiffy) {
      jiffies.emplace_back(jiffy);

    }
  }
  stream.close();
  if (jiffies.size()>21) {
  if (std::all_of(jiffies[13].begin(), jiffies[13].end(), isdigit)) {
      if(jiffies[13].size()!=0) uTime = stol(jiffies[13]);
  }
  if (std::all_of(jiffies[14].begin(), jiffies[14].end(), isdigit)) {
      if(jiffies[14].size()!=0) kTime = stol(jiffies[14]);
  }
  if (std::all_of(jiffies[15].begin(), jiffies[15].end(), isdigit)) {
      if(jiffies[15].size()!=0) cTime = stol(jiffies[15]);
  }
  if (std::all_of(jiffies[16].begin(), jiffies[16].end(), isdigit)) {
      if(jiffies[16].size()!=0) cK = stol(jiffies[16]);
  }
  }
  totalJiffy = uTime + kTime + cTime + cK;

  return totalJiffy/ sysconf(_SC_CLK_TCK);}


long LinuxParser::ActiveJiffies() {  vector<string> jiffies = CpuUtilization();

  long activeJiffies = stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]); 

  return activeJiffies;
  
         }

long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
 }

vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuStats;
  string key;
  string value;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream>>key;
    
    while (linestream >> value) {
      cpuStats.emplace_back(value);
      
      }
      
    }
  }
  filestream.close();
    
      return cpuStats;
    
   }

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }
  filestream.close();
  return 0;
  }

int LinuxParser::RunningProcesses() {  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }
  filestream.close();
  return 0;
  }

string LinuxParser::Command(int pid) { 
  string commandLine;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid)+kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> commandLine;
  }
  stream.close();
  return commandLine;
   }

string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {   
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmData") { // VmData is Physical RAM and Vmsize is Virtal memory size 
          int valueL = stoi(value)*0.001;
          return to_string(valueL) ;
        }
      }
    }
  }
  filestream.close();
  return string(); }

string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key>>value) {
        if (key == "Uid:") {
           return value;
        }
      }
      }
    
  }
  
   return value;}
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  
  string line;
  string userName,xI,uidN;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ' );
      std::istringstream linestream(line);
      while (linestream >> userName >> xI >> uidN) {
        if (uidN == uid) {
          return userName;}
        }

    }
  }
 filestream.close();
  return userName; }

long LinuxParser::UpTime(int pid) { 
  string line;
  string jiffy;
  long upTime{0};
  int counter = 0;
  string startTime;
  long sTime{0};
  std::ifstream stream(kProcDirectory+to_string(pid)+kStatFilename);
  if (stream.is_open()){
    std::getline(stream , line);
    std::istringstream linestream(line);
    while(linestream>>jiffy) {
      
      if (counter==21) startTime = jiffy;
      counter+=1;
    }
    
  }
  stream.close();
  // std::cout<< " the error is here : "<<startTime << "  "<<startTime.size()<<std::endl;
  if (std::all_of(startTime.begin(), startTime.end(), isdigit)) {
      if(startTime.size()!=0)
          sTime = stol(startTime);
  }
    
  upTime=  UpTime()-(sTime/sysconf(_SC_CLK_TCK));
  
 
  return upTime;
  }
