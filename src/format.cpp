#include <string>
#include <iomanip>
#include <sstream>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::ostringstream stream;
    int hour, minutes, second;
    hour = seconds/3600;
    seconds = seconds%3600;
    minutes = seconds/60;
    second = seconds%60;
    stream << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << second;
    return stream.str();
}