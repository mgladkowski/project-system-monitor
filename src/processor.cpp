#include "helpers.h"
#include "linux_parser.h"
#include "processor.h"


/* Constructor
 */
Processor::Processor() {

	vector<string> keys = {"cpu"};
	vector<string> cpus = Helpers::grep(LinuxParser::kProcDirectory + LinuxParser::kStatFilename, keys);
	cores_ = cpus.size()-1;
}


// Return the aggregate CPU utilization
float Processor::Utilization() {

	string key = "cpu";
	vector<string> cpus = Helpers::grep(LinuxParser::kProcDirectory + LinuxParser::kStatFilename, vector<string>{key});

	for (string& line : cpus) {

		unsigned long long user;
		unsigned long long nice;
		unsigned long long system;
		unsigned long long idle;
		unsigned long long iowait;
		unsigned long long irq;
		unsigned long long softirq;
		unsigned long long steal;
		unsigned long long guest;
		unsigned long long guestnice;

		std::istringstream linestream(line);
		linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;

		// credit to htop source code
		unsigned long long total_idle    = idle + iowait;
		unsigned long long total_system  = system + irq + softirq;
		unsigned long long total_virtual = guest + guestnice;
		unsigned long long total_time    = user + nice + total_idle + total_system + steal + total_virtual;
		unsigned long long total_active  = total_time - total_idle;

		unsigned long long delta_active  = total_active - last_active_;
		unsigned long long delta_time    = total_time - last_time_;

		last_active_ = total_active;
		last_time_ = total_time;

		double percent = (delta_time > 0) ? (double)delta_active / delta_time : 0;

		return percent;
	}
	return 0.0;
}