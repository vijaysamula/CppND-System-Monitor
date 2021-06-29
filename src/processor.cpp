#include "processor.h"
#include "linux_parser.h"
#include<iostream>
#include<string>
#include<vector>
float Processor::Utilization() { 
    
    
    long totalCurrent = LinuxParser::Jiffies();
    long idleCurrent = LinuxParser::IdleJiffies();

    float totalD = (float) totalCurrent - (float) TotalPrev();
    float idleD = (float) idleCurrent - (float) IdlePrev();
    updateStep(totalCurrent,idleCurrent);
    float utilization ;
    try {return utilization = (totalD-idleD)/totalD;; }
    catch(...) {utilization= 0;}
    return utilization;
    }


void Processor::updateStep(long total,long idle) {
    TotalPrev(total);
    IdlePrev(idle);
}

void Processor::TotalPrev(long tP){totalPrev_ = tP;}
void Processor::IdlePrev(long iP){idlePrev_ = iP;}
long Processor::TotalPrev(){return totalPrev_;}
long Processor::IdlePrev(){return idlePrev_;} 