#include <vector>
#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

Processor::Processor(){
    previdle_ = 0;
    previowait_ = 0;
    previrq_ = 0;
    prevnice_ = 0;
    prevsystem_ = 0;
    prevsoftirq_ = 0;
    prevuser_ = 0;
    prevsteal_ = 0;

    PrevIdle_ = 0;
    PrevNonIdle_ = 0;
    PrevTotal_ = 0;
    Idle_ = 0;
    NonIdle_ = 0;
    Total_ = 0;
    totald_ = 0;
    idled_ = 0;
}
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> util = LinuxParser::CpuUtilization();
    long user_, nice_, system_, irq_, softirq_, steal_, idle_, iowait_;
    float cpu_Utilization;
    user_ = std::stol(util[0]);
    nice_ = std::stol(util[1]);
    system_ = std::stol(util[2]);
    idle_ = std::stol(util[3]);
    iowait_ = std::stol(util[4]);
    irq_ = std::stol(util[5]);
    softirq_ = std::stol(util[6]);
    steal_ = std::stol(util[7]);
    
    PrevIdle_ = previdle_ + previowait_;
    Idle_ = idle_ + iowait_;

    PrevNonIdle_ = prevuser_ + prevnice_ + prevsystem_ + previrq_ + prevsoftirq_ + prevsteal_;
    NonIdle_ = user_ + nice_ + system_ + irq_ + softirq_ + steal_;

    PrevTotal_ = PrevIdle_ + PrevNonIdle_;
    Total_ = Idle_ + NonIdle_;

    totald_ = Total_ - PrevTotal_;
    idled_ = Idle_ - PrevIdle_;

    cpu_Utilization = float(totald_ - idled_)/ float (totald_);

    prevuser_ = user_;
    prevnice_ = nice_;
    prevsystem_ = system_;
    previdle_ = idle_;
    previowait_ = iowait_;
    previrq_ = irq_;
    prevsoftirq_ = softirq_;
    prevsteal_ = steal_;

    return cpu_Utilization; 
}