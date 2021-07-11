#include <iostream>

#include "filter.h"

using std::cerr;
using std::cout;
using std::sregex_token_iterator;

string htmlBodyAppend(string all) {
    string body = "<body>\n" + all + "</body>\n";
    string ans = "<!doctype html>\n<html>\n" + body + "</html>\n";
    return ans;
}

vector<string> regexSplit(string all, regex re) {
    vector<string> parts(sregex_token_iterator(all.begin(), all.end(), re, -1), sregex_token_iterator());
    return parts;
}

vector<string> regexFindall(string all, regex re) {
    vector<string> parts(sregex_token_iterator(all.begin(), all.end(), re, 0), sregex_token_iterator());
    return parts;
}

void readArg(int argc, char **argv, bool &print, string &inFileName, string &outFileName) {
    string arg = " ";
    for (int i = 1; i < argc; i++) {
        string temp(argv[i]);
        arg = arg + temp + " ";
    }

    print = arg.find(" -p ") != arg.npos;
    bool help = arg.find(" -h ") != arg.npos;

    string helpText = "mdToHTML [options] file\n-h\t\t查看帮助信息\n-o <file>\t指定输出文件名\n-p\t\t在终端打印";
    if (help) {
        cout << helpText;
        exit(0);
    }

    smatch inFileResult, outFileResult;
    bool exactOutFile = regex_search(arg, outFileResult, regex(" -o (\\S+) "));
    string removeOutFile = regex_replace(arg, regex("(.*) -o \\S+ (.*)"), "$1 $2");
    bool exactInFile = regex_search(removeOutFile, inFileResult, regex(" ([^- ]\\S*) "));

    if (!exactInFile) {
        cerr << "open fail";
        exit(1);
    }

    inFileName = inFileResult[1];
    if (exactOutFile)
        outFileName = outFileResult[1];
    else
        outFileName = regex_replace(inFileName, regex("(.+)\\.(?!html).*"), "$1") + ".html";
}
