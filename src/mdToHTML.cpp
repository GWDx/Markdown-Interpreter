#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include "filter.h"

using std::cerr;
using std::cout;
using std::regex_replace;
using std::string;
using std::vector;

string mdToHTML(string raw) {
    string add;
    string add2;
    string ans = "";
    string transferred = regex_replace("\n" + raw, regex("<"), "<+");
    vector<string> codeParts = codeReplaceSplit(transferred);
    for (auto &&codePart : codeParts) {
        if (codePart.find("<pre>") == 0)
            add = codePart;                                     // 代码块
        else {
            add = "";
            vector<string> tableParts = tableReplaceSplit(codePart);
            for (auto &&tablePart : tableParts) {
                if (tablePart.find("<figure>") == 0)
                    add += wordFilter(tablePart);               // 表格
                else
                    add += wordFilter(blockFilter(tablePart));  // 非代码块非表格
            }
        }
        ans += regex_replace(add, regex("<\\+"), "<");
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

    string out = mdToHTML(raw);

    if (print)
        cout << out;
    else {
        std::ofstream outFile(outFileName);
        outFile << out;
        outFile.close();
    }
    return 0;
}
