#ifndef FILTER
#define FILTER

#include <regex>
#include <string>
#include <vector>

using std::regex;
using std::regex_match;
using std::regex_replace;
using std::regex_search;
using std::smatch;
using std::string;
using std::vector;

// wordFilter.cpp
string wordFilter(string line);
string imageReplace(string line);
string linkReplace(string line);
string formatReplace(string line);

// lineFilter.cpp
string lineFilter(string line);
string headReplace(string line);

// blockFilter.cpp
string blockFilter(string all);
string unorderedListReplaceOnce(string all);
string orderedListReplaceOnce(string all);
string quoteReplaceOnce(string all);

// replaceSplit.cpp
vector<string> codeReplaceSplit(string all);
vector<string> tableReplaceSplit(string all);

// other.cpp
string htmlBodyAppend(string all);
vector<string> regexSplit(string all, regex re);
vector<string> regexFindall(string all, regex re);
void readArg(int argc, char **argv, bool &print, string &inFileName, string &outFileName);

// mdToHTML.cpp
string mdToHTML(string raw);

#endif
