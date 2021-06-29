#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hh = seconds/(3600);
    long ss = seconds % (3600);
    long mm = ss/60;
    ss%=60;
    return std::to_string(hh)+':'+std::to_string(mm)+':'+std::to_string(ss); }