#ifndef HELPERS_H
#define HELPERS_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Helpers {

    string grep(const string& filename, const string key = "");
    vector<string> grep(const string& filename, const vector<string> keys);

    string ltrim(const string& value);
    string rtrim(const string& value);
    string trim(const string& value);
    string format_number(const double x, const int digits);

};  // namespace Helpers

#endif