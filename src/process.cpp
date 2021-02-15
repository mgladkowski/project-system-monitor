#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) {
    
    pid_ = pid;
    this->Update();
}


// Return this process's ID
int Process::Pid() { 
    return pid_; 
}


// Return the command that generated this process
string Process::Command() { 
    return command_;
}


// Return the user (name) that generated this process
string Process::User() { 
    return user_;
}


// Return this process's CPU utilization
float Process::CpuUtilization() { 
    return cpu_;
}


// Return this process's memory utilization
string Process::Ram() { 
    return ram_;
}


// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return uptime_;
}


// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    
    // sorts by CPU descending
    return (this->cpu_ > a.cpu_);
}


// Update the object with current stats
void Process::Update() {

    ram_ = LinuxParser::Ram(pid_);
    uptime_ = LinuxParser::UpTime(pid_);
    sys_ticks_ = 0;
    sys_active_ = 0;
    cpu_ = 0.0;
}