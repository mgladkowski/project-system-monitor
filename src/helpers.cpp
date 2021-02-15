#include <math.h>
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
#include <vector>

#include "helpers.h"

using std::string;
using std::vector;


/*
 * File helpers
 */

// Returns first file line containing a keyword
string Helpers::grep(const string& filename, const string key) {

    string output = "";
    string line;

    std::ifstream fs(filename);
    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            if (line.find(key) != std::string::npos)
                return line;
        }
    }
    return output;
}


// Returns all file lines containing any of multiple keywords
vector<string> Helpers::grep(const string& filename, const vector<string> keys) {

    vector<string> output = {};
    string line;

    std::ifstream fs(filename);
    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            for (const string& key : keys) {
                if (line.find(key) != std::string::npos) {
                    output.push_back(line);
                }
            }
        }
    }
    return output;
}


/*
 * String helpers
 * Credit for time savings to : 
 * https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
 * https://stackoverflow.com/questions/1343890/how-do-i-restrict-a-float-value-to-only-two-places-after-the-decimal-point-in-c
 */

string Helpers::ltrim(const string& s) {
    return std::regex_replace(s, std::regex("^\\s+"), string(""));
}


string Helpers::rtrim(const string& s) {
    return std::regex_replace(s, std::regex("\\s+$"), string(""));
}


string Helpers::trim(const string& s) {
    return ltrim(rtrim(s));
}


string Helpers::format_number(const double x, const int digits) {
    std::stringstream ss;
    ss << std::fixed;
    ss.precision(digits);
    ss << x;
    return ss.str();
}