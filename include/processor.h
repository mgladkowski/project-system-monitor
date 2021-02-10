#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:

    Processor();
    float Utilization();
    int Cores() { return cores_; };

 private:

    int cores_ { 0 };
    unsigned long last_active_ { 0 };
    unsigned long last_time_   { 0 };
};

#endif