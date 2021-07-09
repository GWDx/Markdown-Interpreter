#ifndef FILTER
#define FILTER

#include <string>
#include <vector>
#include <regex>

using std::string;
using std::vector;
using std::regex;

// wordFilter
string wordFilter(string line);
string imageReplace(string line);
string linkReplace(string line);
string formatReplace(string line);

// lineFilter
string lineFilter(string line);
string headReplace(string line);

// blockFilter
string blockFilter(string all);
vector<string> codeReplaceSplit(string all);
string unorderedListReplace(string all);
string orderedListReplace(string all);
string quoteReplace(string all);
vector<string> tableReplaceSplit(string all);

// other
string htmlBodyAppend(string all);
vector<string> split(string all, regex re);
vector<string> findall(string all, regex re);
void readArg(int argc, char **argv, bool &print, string &inFileName, string &outFileName);

#endif
