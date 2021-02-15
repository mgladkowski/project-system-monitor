#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <array>
#include <map>
#include <iterator>
#include <string>
#include <vector>

using std::array;
using std::map;
using std::iterator;
using std::stof;
using std::string;
using std::to_string;
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

map<string, array<unsigned long long,2>> CpuUtilization();

// Processes
string Command(int pid);
string Uid(int pid);
string User(int pid);
string Ram(int pid);
unsigned long long CpuTime(int pid);
unsigned long long StartTime(int pid);

};  // namespace LinuxParser

#endif