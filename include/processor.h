#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

#include <array>
#include <map>

using std::array;
using std::map;

class Processor {
 public:

	vector<float> Utilization();

 private:

	map<string, unsigned long long> total_ {};
	map<string, unsigned long long> active_ {};
	vector<float> utilization_ {};
};

#endif