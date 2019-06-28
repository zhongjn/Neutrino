#pragma once
#include <memory>
#include <string>
#include <sstream>
#include <stdarg.h>

using namespace std;

string string_format(const string fmt_str, ...);

inline void replace_str(string& str, const string& from, const string& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

inline string quote(const string& t) {
    stringstream ss;
    string tt = t;
    replace_str(tt, "\"", "\"\"");
    ss << "\"" << tt << "\"";
    return ss.str();
}
