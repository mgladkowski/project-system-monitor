#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

using std::string;
using std::vector;

namespace LinuxParser {

// Paths
const string kProcDirectory{"/proc/"};
const string kCmdlineFilename{"/cmdline"};
const string kCpuinfoFilename{"/cpuinfo"};
const string kStatusFilename{"/status"};
const string kStatFilename{"/stat"};
const string kUptimeFilename{"/uptime"};
const string kMeminfoFilename{"/meminfo"};
const string kVersionFilename{"/version"};
const string kOSPath{"/etc/os-release"};
const string kPasswordPath{"/etc/passwd"};

// System
vector<int>   Pids();
string        Kernel();
string        OperatingSystem();
float         MemoryUtilization();
unsigned int  RunningProcesses();
unsigned int  TotalProcesses();
unsigned long UpTime();

// CPU
enum CPUStates {
	kUser_ = 0,
	kNice_,
	kSystem_,
	kIdle_,
	kIOwait_,
	kIRQ_,
	kSoftIRQ_,
	kSteal_,
	kGuest_,
	kGuestNice_
};

vector<string> CpuUtilization();
unsigned long Jiffies();
unsigned long ActiveJiffies();
unsigned long ActiveJiffies(int pid);
unsigned long IdleJiffies();

// Processes
string Command(int pid);
string Ram(int pid);
string Uid(int pid);
string User(int pid);
unsigned long UpTime(int pid);

};  // namespace LinuxParser

#endif