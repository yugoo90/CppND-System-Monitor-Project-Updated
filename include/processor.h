#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   long prevuser_, previdle_, previowait_, prevnice_, prevsystem_, previrq_, prevsoftirq_, prevsteal_;
   long Idle_, PrevIdle_, NonIdle_, PrevNonIdle_, PrevTotal_, Total_, totald_, idled_;
   
};

#endif