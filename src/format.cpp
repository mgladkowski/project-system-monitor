#include <iomanip>
#include <string>
#include <sstream>

#include "format.h"

/* Return an elapsed time string from integer seconds
    INPUT  : Long int measuring seconds
    OUTPUT : HH:MM:SS
*/
std::string Format::ElapsedTime(long seconds) {

    // validate input - invalid values are zeroed

    if (seconds < 0) seconds = 0;

    // individual time segments

    const int hh = seconds / 3600;
    const int mm = (seconds - hh*3600) / 60;
    const int ss = (seconds - hh*3600 - mm*60);

    // format the time using a stringstream

    std::stringstream ssout;
    
    ssout << std::setfill('0') 
        << std::setw(2) << hh << ':' 
        << std::setw(2) << mm << ':' 
        << std::setw(2) << ss;
   
    return ssout.str();
}