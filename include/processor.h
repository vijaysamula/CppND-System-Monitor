#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  void updateStep(long total,long idle);
  void TotalPrev(long tP);
  long TotalPrev();
  void IdlePrev(long iP);
  long IdlePrev();
  // TODO: Declare any necessary private members
 private:
    long totalPrev_{0},totalCurrent_{0},idlePrev_ {0},idleCurrent_{0};
};

#endif