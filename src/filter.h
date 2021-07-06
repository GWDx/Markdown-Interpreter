#ifndef FILTER
#define FILTER

#include <regex>
#include <string>
#include <vector>

using std::regex;
using std::regex_replace;
using std::string;
using std::vector;

string wordFilter(string line);
string imageReplace(string line);
string linkReplace(string line);
string boldReplace(string line);
string italicReplace(string line);

string lineFilter(string line);
string headReplace(string line);

string blockFilter(string all);
string unorderedListReplace(string all);
string orderedListReplace(string all);
string codeReplace(string all);
string quoteReplace(string all);

string htmlBodyAppend(string all);
vector<string> split(string all);
void readArg(int argc, char **argv, bool &print, string &inFileName, string &outFileName);

#endif
