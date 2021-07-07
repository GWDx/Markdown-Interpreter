#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "filter.h"

using std::cerr;
using std::cout;
using std::string;
using std::vector;

int main(int argc, char **argv) {
    bool print;
    string inFileName, outFileName;
    readArg(argc, argv, print, inFileName, outFileName);
    cout << inFileName << '\n';
    cout << outFileName << '\n';
    std::ifstream inFile;
    std::stringstream buffer;
    inFile.open(inFileName);
    if (inFile.fail()) {
        cerr << "open fail";
        exit(1);
    }
    buffer << inFile.rdbuf();
    inFile.close();
    string raw = buffer.str();

    vector<string> lines = split(blockFilter(raw));
    string ans = "";
    string add;
    for (auto &&line : lines) {
        if (line.find("<pre>") == 0 || line == "")
            add = line;
        else if (line.find("<ul>") == 0 || line.find("<blockquote>") == 0)
            add = wordFilter(line);
        else
            add = lineFilter(wordFilter(line));
        ans = ans + add + "\n";
    }
    string out = htmlBodyAppend(ans);

    if (print)
        cout << out;
    else {
        std::ofstream outFile(outFileName);
        outFile << out;
        outFile.close();
    }
    return 0;
}
