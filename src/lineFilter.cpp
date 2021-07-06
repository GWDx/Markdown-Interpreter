#include "filter.h"

string lineFilter(string line) {
    string result = headReplace(line);
    if (result != "")
        return result;
    return "<p>" + line + "</p>";
}

string headReplace(string line) {
    // re.match(r'^(#{1,6}) (.*)', line)
    regex r1("(#{1,6}) (.+)");
    std::smatch result;
    bool matched = std::regex_match(line, result, r1);
    if (!matched)
        return "";
    string length = std::to_string(result[1].length());
    string content = result[2];
    string ans = "<h" + length + " id='" + content + "'>" + content + "</h" + length + ">";
    return ans;
}
