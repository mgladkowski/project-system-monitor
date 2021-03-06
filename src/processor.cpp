#include "processor.h"

/* Return the CPU utilization
 *
 * Returns vector of utilization per core, first value is aggregate
 */
vector<float> Processor::Utilization() {

	utilization_.clear();

	// parser returns a map containing per core jiffy data
	map<string, array<unsigned long long,2>> cores = LinuxParser::CpuUtilization();
	map<string, array<unsigned long long,2>>::iterator it = cores.begin();
	while ( it != cores.end()) {

		// retrieve key and data from returned map
		string key = it->first;
		array<unsigned long long,2> data = it->second;

		// difference from last used values
		const unsigned long long delta_time   = data[0] - total_[key];
		const unsigned long long delta_active = data[1] - active_[key];

		// utilization computed from difference since last queried
		const float used = (delta_time > 0) ? static_cast<float>(delta_active) / delta_time : 0.0;
		utilization_.push_back(used);
		
		// update last used values
		total_[key] = data[0];
		active_[key] = data[1];

		it++;
	}
	return utilization_;
}