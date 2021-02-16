#include <dirent.h>
#include <unistd.h>
#include "helpers.h"
#include "linux_parser.h"


// Read and return operating system kernel
string LinuxParser::Kernel() {

	string value = "";
	string cut;
	string line = Helpers::grep(kProcDirectory + kVersionFilename);

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


/* Read and return CPU utilization
 *
 * Returns a vector of cpu data:
 * First item is expected to be aggregate
 * Subsequent items are individual cores
 * 
 * First array index is time_total
 * Second array index is time_active
 */
map<string, array<unsigned long long,2>> LinuxParser::CpuUtilization() {

	map<string, array<unsigned long long, 2>> output = {};

	vector<string> lines = Helpers::grep(kProcDirectory + kStatFilename, vector<string>{"cpu"});

	// parsing all CPU lines so we have each core as well
	for (string& line : lines) {

		// extract jiffy data from line

		string key = "";
		unsigned long long t[10];

		std::istringstream linestream(line);
		linestream >> key >> t[kUser_] 
						  >> t[kNice_] 
						  >> t[kSystem_]
						  >> t[kIdle_]
						  >> t[kIOwait_]
						  >> t[kIRQ_]
						  >> t[kSoftIRQ_]
						  >> t[kSteal_] 
						  >> t[kGuest_]
						  >> t[kGuestNice_];

		// used formulas from htop source code

		unsigned long long total_idle    = t[kIdle_] + t[kIOwait_];
		unsigned long long total_system  = t[kSystem_] + t[kIRQ_] + t[kSoftIRQ_];
		unsigned long long total_virtual = t[kGuest_] + t[kGuestNice_];

		// add to return map

		unsigned long long time_total  = t[kUser_] + t[kNice_] + total_idle + total_system + t[kSteal_] + total_virtual;
		unsigned long long time_active = time_total - total_idle;
		array<unsigned long long,2> data = {time_total, time_active};
		output[key] = data;
	}
	return output;
}


// Read and return the command associated with a process
string LinuxParser::Command(int pid) {

	return Helpers::grep(kProcDirectory + to_string(pid) + kCmdlineFilename);
}


// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 

	string key = "Uid:";
	string value = "";
	string line = Helpers::grep(kProcDirectory + to_string(pid) + kStatusFilename, key);

	std::istringstream linestream(line);
	linestream >> key >> value;
	return value;
}


// Read and return the user associated with a process
string LinuxParser::User(int pid) { 

	string value = "";
	string line = Helpers::grep(kPasswordPath, ":x:"+Uid(pid)+":");

	std::replace(line.begin(), line.end(), ':', ' ');
	std::istringstream linestream(line);
	linestream >> value;
	return value;
}


// Read and return the memory used by the process in MB
string LinuxParser::Ram(int pid) {

	string line = Helpers::grep(kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:");
	std::istringstream linestream(line);
	
	string word;
	long value;
	linestream >> word >> value;
	return Helpers::format_number((double)value/1024.0, 1);
}


// Read and return cpu clock ticks elapsed of this process
unsigned long long LinuxParser::CpuTime(int pid) {

	string line = Helpers::grep(kProcDirectory + to_string(pid) + kStatFilename);
	std::istringstream linestream(line);
	
	string word;
	unsigned long long utime, stime, cutime, cstime;
	int i = 0;
    while (linestream >> word) {
        i++;
		if (i == 13) {
			linestream >> utime >> stime >> cutime >> cstime;
			break;
		}
	}
	return utime + stime + cutime + cstime;
}


// Read and return starting uptime of this process
unsigned long long LinuxParser::StartTime(int pid) {

	string line = Helpers::grep(kProcDirectory + to_string(pid) + kStatFilename);
	std::istringstream linestream(line);
	
	string word;
	unsigned long long starttime;
	int i = 0;
    while (linestream >> word) {
        i++;
		if (i == 21) {
			linestream >> starttime;
			break;
		}
	}
	return starttime;
}