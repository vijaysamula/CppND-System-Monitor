#include <string>
#include <iomanip>
#include "format.h"
#include <sstream>

using std::string;

string Format::ElapsedTime(long seconds) { 
    long hh = seconds/(3600);
    long ss = seconds % (3600);
    long mm = ss/60;
    ss%=60;
    std::stringstream format;
    format << std::setfill('0') << std::setw(2) << hh << ":"
         << std::setfill('0') << std::setw(2) << mm << ":"
         << std::setfill('0') << std::setw(2) << ss; 
    return format.str();     }