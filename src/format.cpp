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

    int hh = (int)seconds / 3600;
    int mm = (int)(seconds - hh*3600) / 60;
    int ss = (int)(seconds - hh*3600 - mm*60);

    // format the time using a stringstream

    std::stringstream ssout;
    
    ssout << std::setfill('0') 
        << std::setw(2) << hh << ':' 
        << std::setw(2) << mm << ':' 
        << std::setw(2) << ss;
   
    // return the string

    return ssout.str();
}