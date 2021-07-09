#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "filter.h"

using std::cerr;
using std::cout;
using std::regex_replace;
using std::string;
using std::vector;

string transform(string raw) {
    string add;
    string add2;
    string ans = "";
    vector<string> codeParts = codeReplaceSplit("\n" + raw);
    for (auto &&codePart : codeParts) {
        string trans = regex_replace(codePart, regex("<"), "<|");
        if (codePart.find("<pre>") == 0)
            add = trans;
        else {
            add = "";
            vector<string> tableParts = tableReplaceSplit(trans);
            for (auto &&tablePart : tableParts) {
                if (tablePart.find("<figure>") == 0)
                    add += tablePart;
                else
                    add += wordFilter(blockFilter(tablePart));
            }
        }
        ans += regex_replace(add, regex("<\\|"), "<");
    }
    string out = htmlBodyAppend(ans);
    return out;
}

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

    string out = transform(raw);

    if (print)
        cout << out;
    else {
        std::ofstream outFile(outFileName);
        outFile << out;
        outFile.close();
    }
    return 0;
}
