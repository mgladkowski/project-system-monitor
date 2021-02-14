#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "helpers.h"
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


// Read and return operating system kernel
string LinuxParser::Kernel() {

	string value = "";
	string cut;
	string line = Helpers::grep(kProcDirectory + kVersionFilename, "");

	std::istringstream linestream(line);
	linestream >> cut >> cut >> value;
	return value;
}


// Read and return operating system name
string LinuxParser::OperatingSystem() {

	string value = "";
	string cut;
	string line = Helpers::grep(kOSPath, "PRETTY_NAME");

	std::replace(line.begin(), line.end(), ' ', '_');
	std::replace(line.begin(), line.end(), '"', ' ');
	std::replace(line.begin(), line.end(), '=', ' ');
	std::istringstream linestream(line);
	while (linestream >> cut >> value) {
		std::replace(value.begin(), value.end(), '_', ' ');
		return value;
	}
	return value;
}


// Read and return a vector of process IDs
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


// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {

	vector<string> keys = {"MemTotal","MemFree"};
	vector<string> lines = Helpers::grep(kProcDirectory + kMeminfoFilename, keys);

	double memTotal = 0;
	double memFree = 0;
	float  memUsedPercent = 0;

	for (string& line : lines) {
		string key, value, unit;
		std::istringstream linestream(line);
		linestream >> key >> value >> unit;
		if (key == "MemTotal:") memTotal = strtod(value.c_str(), NULL);
		if (key == "MemFree:") memFree = strtod(value.c_str(), NULL);
	}
	memUsedPercent = (memTotal > 0) ? (memTotal - memFree) / memTotal : 0;

	return memUsedPercent;
}


// Read and return the number of running processes
unsigned int LinuxParser::RunningProcesses() {

	string value = "";
	string cut;
	string line = Helpers::grep(kProcDirectory + kStatFilename, "procs_running");

	std::istringstream linestream(line);
	linestream >> cut >> value;
	return strtoul(value.c_str(), NULL, 0);
}


// Read and return the total number of processes
unsigned int LinuxParser::TotalProcesses() {

	string value = "";
	string cut;
	string line = Helpers::grep(kProcDirectory + kStatFilename, "processes");

	std::istringstream linestream(line);
	linestream >> cut >> value;
	return strtoul(value.c_str(), NULL, 0);
}


// Read and return the system uptime
unsigned long LinuxParser::UpTime() {

	string value = "";
	string line = Helpers::grep(kProcDirectory + kUptimeFilename, "");

	std::istringstream linestream(line);
	linestream >> value;
	return strtoul(value.c_str(), NULL, 0);
}


// Read and return the number of jiffies for the system
unsigned long LinuxParser::Jiffies() {

	string value = "";
	string cut;
	string line = Helpers::grep(kProcDirectory + kStatFilename, "processes");

	std::istringstream linestream(line);
	linestream >> cut >> value;
	return strtoul(value.c_str(), NULL, 0);
}


// Read and return the number of active jiffies for a PID
unsigned long LinuxParser::ActiveJiffies(int pid) {

	return 0;
}


// Read and return the number of active jiffies for the system
unsigned long LinuxParser::ActiveJiffies() {

	return 0;
}


// Read and return the number of idle jiffies for the system
unsigned long LinuxParser::IdleJiffies() {

	return 0;
}


// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
	return {};
}


// Read and return the command associated with a process
string LinuxParser::Command(int pid) { return string(); }


// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { return string(); }


// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { return string(); }


// Read and return the user associated with a process
string LinuxParser::User(int pid) { return string(); }


// Read and return the uptime of a process
unsigned long LinuxParser::UpTime(int pid) { return 0; }
