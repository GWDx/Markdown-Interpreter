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
string wordFilter(string part); // 替换图片、链接、文字格式
string imageReplace(string part);
string linkReplace(string part);
string formatReplace(string part);

// lineFilter.cpp
string lineFilter(string line); // 替换一行的内容（若不是标题则加 <p>）
string headReplace(string line);

// blockFilter.cpp
string blockFilter(string all); // 替换非代码块非表格区域中的列表、引用，并处理行的格式
string unorderedListReplaceOnce(string all);    // 替换一层缩进 <=3 个空格的无序列表
string orderedListReplaceOnce(string all);
string quoteReplaceOnce(string all);

// replaceSplit.cpp
vector<string> codeReplaceSplit(string all);    // 替换代码块，并切分为代码块和非代码块
vector<string> tableReplaceSplit(string all);

// other.cpp
string htmlBodyAppend(string all);
vector<string> regexSplit(string all, regex re);
vector<string> regexFindall(string all, regex re);
void readArg(int argc, char **argv, bool &print, string &inFileName, string &outFileName);

// mdToHTML.cpp
string mdToHTML(string raw);    // 输入markdown 代码，输出 HTML 代码

#endif
