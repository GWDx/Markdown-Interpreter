#ifndef FILTER
#define FILTER

#include <regex>
#include <string>
#include <vector>

using std::regex;
using std::regex_replace;
using std::string;
using std::vector;

// wordFilter
string wordFilter(string line);
string imageReplace(string line);
string linkReplace(string line);
string boldItalicReplace(string line);
string strikethroughReplace(string line);

// lineFilter
string lineFilter(string line);
string headReplace(string line);

// blockFilter
string blockFilter(string all);
string unorderedListReplace(string all);
string orderedListReplace(string all);
string codeReplace(string all);
string quoteReplace(string all);

// other
string htmlBodyAppend(string all);
vector<string> split(string all);
void readArg(int argc, char **argv, bool &print, string &inFileName, string &outFileName);

#endif
