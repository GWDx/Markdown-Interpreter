#include <iostream>
#include <regex>

#include "filter.h"

using std::cerr;
using std::cout;
using std::regex;
using std::regex_replace;
using std::smatch;
using std::sregex_token_iterator;

string htmlBodyAppend(string all) {
    string body = "<body>\n" + all + "</body>\n";
    string ans = "<!doctype html>\n<html>\n" + body + "</html>\n";
    return ans;
}

vector<string> split(string all, regex re) {
    vector<string> parts(sregex_token_iterator(all.begin(), all.end(), re, -1), sregex_token_iterator());
    return parts;
}

vector<string> findall(string all, regex re) {
    vector<string> parts(sregex_token_iterator(all.begin(), all.end(), re, 0), sregex_token_iterator());
    return parts;
}

// vector<string> split(string all) {
//     // ans = re.findall(r'<(?:pre|ul|blockquote)>[\s\S]*?<\/(?:pre|ul|blockquote)>|\n|.+', replaced)
//     regex r1("<(pre|ul|blockquote)>[\\s\\S]*?<\\/\\1>|.+");
//     vector<string> ans(sregex_token_iterator(all.begin(), all.end(), r1, 0), sregex_token_iterator());
//     return ans;
// }

void readArg(int argc, char **argv, bool &print, string &inFileName, string &outFileName) { // ? simplify
    string arg = " ";
    for (int i = 1; i < argc; i++) {
        string temp(argv[i]);
        arg = arg + temp + " ";
    }

    print = arg.find("-p ") != arg.npos;
    bool help = arg.find("-h ") != arg.npos;

    string helpText = "mdToHTML [options] file\n-h\t\t查看帮助信息\n-o <file>\t指定输出文件名\n-p\t\t在终端打印";
    if (help) {
        cout << helpText;
        exit(0);
    }

    smatch regexResult1, regexResult2;
    bool exactOutFile = regex_search(arg, regexResult2, regex(" -o (\\S+) "));
    string removeOutFile = regex_replace(arg, regex("(.*) -o \\S+ (.*)"), "$1 $2");
    bool exactInFile = regex_search(removeOutFile, regexResult1, regex(" ([^- ]\\S+) "));

    if (!exactInFile) {
        cerr << "open fail";
        exit(1);
    }

    inFileName = regexResult1[1];
    if (exactOutFile)
        outFileName = regexResult2[1];
    else
        outFileName = regex_replace(inFileName, regex("(.+)\\.(?!html$).*"), "$1") + ".html";
}
