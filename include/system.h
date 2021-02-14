#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::string;
using std::vector;

class System {
 public:

	Processor&       Cpu();
	vector<Process>& Processes();
	string           Kernel();
	string           OperatingSystem();
	float            MemoryUtilization();
	unsigned int     RunningProcesses();
	unsigned int     TotalProcesses();
	unsigned long    UpTime();

 private:

	Processor        cpu_;
	vector<Process>  processes_;
};

#endif