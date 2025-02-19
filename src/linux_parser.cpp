#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float total = 1, free = 1;
  string line, key;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream lineStream(line);
      lineStream >> key;
      if(key == "MemTotal:"){
        lineStream >> total;
      }
      else if(key == "MemAvailable:"){
        lineStream >> free;
        break;
      }
    }
  }
  return (total - free) / total; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string running, idle, line;
  long upTime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> running >> idle;
    upTime = std::stol(running);
  }
  return upTime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long user, nice, system, idle, IOWait, irq, softIrq, steal;
  long jiffies, activeJiffies, idleJiffies;
  string cpu, line, kUser, kNice, kSystem, kIdle, kIOWait, kIrq, kSoftIrq, kSteal;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    //std::ofstream myFile;
    //myFile.open("file.txt");
    //myFile << line;
    linestream >> cpu >> kUser >> kNice >> kSystem >> kIdle >> kIOWait >> kIrq >> kSoftIrq >> kSteal;
    user = std::stol(kUser);
    nice = std::stol(kNice);
    system = std::stol(kSystem);
    idle = std::stol(kIdle);
    IOWait = std::stol(kIOWait);
    irq = std::stol(kIrq);
    softIrq = std::stol(kSoftIrq);
    steal = std::stol(kSteal);
    //myFile.close();
    activeJiffies = user + nice + system + irq + softIrq + steal;
    idleJiffies = idle + IOWait;
    jiffies = activeJiffies + idleJiffies;
  }
  
  return jiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 

  long activeJiffies;
  string line, jiffy;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> jiffy){
        jiffies.push_back(jiffy);
      }
      
    }
    for(int counter = 13; counter < 17; counter++){
      activeJiffies += std::stol(jiffies[counter]);
    }
  }
  return activeJiffies / sysconf(_SC_CLK_TCK); 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  long user, nice, system, irq, softIrq, steal;
  string line, kUser, kNice, kSystem, kIrq, kSoftIrq, kSteal;
  long activeJiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> kUser >> kNice >> kSystem >> kIrq >> kSoftIrq >> kSteal;
    user = std::stol(kUser);
    nice = std::stol(kNice);
    system = std::stol(kSystem);
    irq = std::stol(kIrq);
    softIrq = std::stol(kSoftIrq);
    steal = std::stol(kSteal);
    activeJiffies = user + nice + system + irq + softIrq + steal;
  }

  return activeJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  long idle, IOWait, idleJiffies;
  string line, kUser, kNice, ksystem, kIdle, kIOWait, kIrq, kSoftIrq, kSteal;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> kUser >> kNice >> ksystem >> kIdle >> kIOWait >> kIrq >> kSoftIrq >> kSteal;
    idle = std::stol(kIdle);
    IOWait = std::stol(kIOWait);
    idleJiffies = idle + IOWait;
  }
  return idleJiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuUtil;
  string line, cpu, user, nice, system, idle, softIrq, steal, guest, guest_Nice, irq, IOwait;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> IOwait >> irq >> softIrq >> steal >> guest >> guest_Nice;
    cpuUtil.push_back(user);
    cpuUtil.push_back(nice);
    cpuUtil.push_back(system);
    cpuUtil.push_back(idle);
    cpuUtil.push_back(IOwait);
    cpuUtil.push_back(irq);
    cpuUtil.push_back(softIrq);
    cpuUtil.push_back(steal);
    cpuUtil.push_back(guest);
    cpuUtil.push_back(guest_Nice);
  }
  return cpuUtil; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  int totalProcesses;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "processes"){
          totalProcesses = std::stoi(value);
        }
      }
    }
  }
  return totalProcesses; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  int runningProcesses;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "procs_running"){
          runningProcesses = std::stoi(value);
        }
      }
    }
  }
  return runningProcesses; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> command;
  }
  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  string ram = "0";
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "VmSize:"){
          ram = value;
        }
      }
      int num = std::stoi(ram);
      num = num / 1024;
      ram = std::to_string(num);
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, UID, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> UID >> value){
        if(UID == "Uid:"){
          return value;
        }
      }
    }
  } 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, value, x , id;
  string name = "Default";
  string uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> value >> x >> id;
      if(id == uid){
        name = value;
        break;
      }
    }
  }
  return name;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, value;
  long upTime;
  int counter = 21;
  vector<string> info;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> value){
        info.push_back(value);
      }
    }
    upTime = std::stol(info[counter]);
  }
  return upTime / sysconf(_SC_CLK_TCK); 
}
