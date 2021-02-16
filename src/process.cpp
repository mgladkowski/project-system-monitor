#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

#include <iostream>

using std::string;
using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid) {
    
    pid_ = pid;
    command_ = LinuxParser::Command(pid_);
    user_ = LinuxParser::User(pid_);
    start_time_ = LinuxParser::StartTime(pid_);
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


/* Update the object with current stats
 * Requires the current system uptime in seconds
 */
void Process::Update() {

    // ram
    ram_ = LinuxParser::Ram(pid_);

    // cpu time
    const unsigned long long cpu_time = LinuxParser::CpuTime(pid_);
    const unsigned long long cpu_used = cpu_time - last_cpu_time_;

    /* elapsed time since process spawned
     * unclear whether this was supposed to be CPU wall time */
    const unsigned long system_uptime_s = LinuxParser::UpTime();
    unsigned long long system_time = system_uptime_s * sysconf(_SC_CLK_TCK);
    if (system_time < start_time_) system_time = start_time_;

    const unsigned long long elapsed_time = system_time - last_sys_time_;
    const unsigned long long process_time = system_time - start_time_;
    uptime_ = process_time / sysconf(_SC_CLK_TCK);

    // cpu utilization
    cpu_ = (elapsed_time > 0) ? static_cast<double>(cpu_used) / elapsed_time : 0.0;
    if (cpu_ > 1.0) cpu_ = 1.0;  // constrain edge cases when CPU walled at 100%

    // store current values
    last_cpu_time_ = cpu_time;
    last_sys_time_ = system_time;
}