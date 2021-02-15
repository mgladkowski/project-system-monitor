#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::string;
using std::vector;


// Return the system's CPU
Processor& System::Cpu() {
    return cpu_;
}


// Return a container composed of the system's processes
vector<Process>& System::Processes() {

    vector<Process> output;

    // fetch current PID list
    vector<int> pids = LinuxParser::Pids();

    // add or update valid processes, keeping existing ones
    for (int& pid : pids) {

        auto it = std::find_if(
            std::begin(processes_), 
            std::end(processes_),
            [pid](Process& element) -> bool
            { return element.Pid() == pid; });

        if (it != processes_.end()) {
            it->Update();
            output.push_back(*it);
        } else
            output.push_back(Process(pid));
    }

    // sort and store valid processes
    std::sort(output.begin(), output.end());
    processes_ = output;

    return processes_;
}


// Return the system's kernel identifier
string System::Kernel() {
    return LinuxParser::Kernel();
}


// Return the operating system name
string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}


// Return the system's memory utilization
float System::MemoryUtilization() {
    return LinuxParser::MemoryUtilization();
}


// Return the number of processes actively running on the system
unsigned int System::RunningProcesses() {
    return LinuxParser::RunningProcesses();
}


// Return the total number of processes on the system
unsigned int System::TotalProcesses() {
    return LinuxParser::TotalProcesses();
}


// Return the number of seconds since the system started running
unsigned long System::UpTime() {
    return LinuxParser::UpTime();
}