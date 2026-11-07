//purpose: Shared parsing and matching helpers
#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
#include <vector>
#include <cctype>
#include <iostream>
namespace util {
//trim surrounding whitespace, including Windows carriage returns.
inline std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b-a);
}
//case-normalize ASCII letters for comparisons and facility IDs.
inline std::string upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}
inline std::string normalized(const std::string& s) { return upper(trim(s)); }
//split a complete line, preserving empty fields.
inline std::vector<std::string> split(const std::string& s, char delim='|') {
    std::vector<std::string> result; size_t start = 0, end;
    while ((end = s.find(delim,start)) != std::string::npos) {
        result.push_back(trim(s.substr(start,end-start))); start=end+1;
    }
    result.push_back(trim(s.substr(start)));
    return result;
}
inline bool skip(const std::string& s) {
    std::string t=trim(s); return t.empty() || t[0]=='#';
}
inline void warn(const std::string& file, int line, const std::string& reason) {
    std::cout << "Warning: " << file << ":" << line << ": " << reason << '\n';
}
inline bool validID(const std::string& id) {
    return id.size()==4 && id[0]=='F' && id[1]>='0' && id[1]<='9' &&
           id[2]>='0' && id[2]<='9' && id[3]>='0' && id[3]<='9';
}
}
#endif

