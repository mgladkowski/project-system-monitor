#include <fstream>
#include <regex>
#include <string>
#include <vector>

#include "helpers.h"

using std::string;
using std::vector;


/*
 * File helpers
 */

// Returns file lines containing any of multiple keywords
vector<string> Helpers::grep(const string filename, const vector<string> keys) {

    vector<string> output;
    string line;

    std::ifstream fs(filename);
    if (fs.is_open()) {
        while (std::getline(fs, line)) {
            for (const string& key : keys) {
                if (line.find(key))
                    output.push_back(line.c_str());
            }
        }
    }
    return output;
}


/*
 * String helpers
 * Credit to : https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
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
