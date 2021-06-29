#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    long hh = seconds/(3600);
    long ss = seconds % (3600);
    long mm = ss/60;
    ss%=60;
    return std::to_string(hh)+':'+std::to_string(mm)+':'+std::to_string(ss); }