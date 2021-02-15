#ifndef PROCESS_H
#define PROCESS_H

#include <string>

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:

    Process(int pid);

    int      Pid();
    string   User();
    string   Command();
    float    CpuUtilization();
    string   Ram();
    long int UpTime();

    bool operator<(Process const& a) const;

    void     Update(unsigned long int system_uptime_s);

 private:

    int      pid_ {0};
    string   user_ {""};
    string   command_ {""};
    float    cpu_ {0.0};
    string   ram_ {""};
    unsigned long uptime_ {0};

    unsigned long long last_cpu_time_ {0};
    unsigned long long last_sys_time_ {0};
    unsigned long long start_time_ {0};
};

#endif