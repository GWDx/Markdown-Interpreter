#include <iostream>
#include <regex>

#include "filter.h"

using std::regex;
using std::regex_match;
using std::regex_replace;
using std::regex_search;
using std::smatch;
using std::sregex_token_iterator;

string blockFilter(string all) { // ? nest
    string replaced = all;
    if (!regex_search(all, regex("<ul|ol|blockquote|li")))
        replaced = quoteReplace(orderedListReplace(unorderedListReplace(all)));
    regex r1("<(ul|ol|blockquote|li)[\\s\\S]*?<\\/\\1>|.+");
    vector<string> lines = findall(replaced, r1);
    // if (lines.size() == 1)
    //     return lineFilter(replaced);
    string add;
    string ans = "";
    smatch regexResult;
    for (auto &&line : lines) {
        if (regex_match(line, regexResult, regex("<((ul|ol|blockquote|li).*?)>([\\s\\S]*)<\\/\\2>"))) {
            string label1 = regexResult[1];
            string label2 = regexResult[2];
            string rawContent = regexResult[3];
            string content = blockFilter("\n" + rawContent + "\n\n\n");
            add = "<|" + label1 + ">" + content + "<|/" + label2 + ">";
        } else
            add = lineFilter(line);
        ans += add + "\n";
    }
    return ans;
}

vector<string> codeReplaceSplit(string all) {
    string ans = regex_replace(all, regex("\\n```(.*)([\\s\\S]*?)```\\n"), "\n<pre><code lang='$1'>$2</code></pre>\n");
    // regex r1("(?=<pre>)|(?<=\\/pre>)");
    // vector<string> parts = split(ans, r1);
    regex r1("<pre>[\\s\\S]*?<\\/pre>|[\\s\\S]+?(?=<pre>)|[\\s\\S]+");
    vector<string> parts = findall(ans, r1);
    return parts;
}

string unorderedListReplace(string all) {
    regex r1("\\n {0,3}[+-] (.*(\\n{1,2}  .*)*)(\\n{1,2} {0,3}[+-] (.*(\\n{1,2}  .*)*))*");
    regex r2("\\n {0,3}[+-] (.*(\\n{1,2}  .*)*)");
    string addUL = regex_replace(all, r1, "\n<ul>\n$0\n\n\n</ul>");
    string addLI = regex_replace(addUL, r2, "\n<li>$1\n\n\n</li>");
    return addLI;
}

string orderedListReplace(string all) {
    regex r1("\\n {0,3}(\\d+)\\. (.*(\\n{1,2}  .*)*)(\\n{1,2} {0,3}\\d+\\. (.*(\\n{1,2}  .*)*))*");
    regex r2("\\n {0,3}\\d+\\. (.*(\\n{1,2}  .*)*)");
    string addOL = regex_replace(all, r1, "\n<ol start='$1'>\n$0\n\n\n</ol>");
    string addLI = regex_replace(addOL, r2, "\n<li>$1\n\n\n</li>");
    return addLI;
}

string quoteReplace(string all) {
    string addBlockquote = regex_replace(all, regex("((\\n(<p>)? {0,3}>.*)+)"), "\n\n\n<blockquote>$1\n</blockquote>");
    string addP = regex_replace(addBlockquote, regex("\\n(<p>)? {0,3}>(.*?)(<\\/p>)?(?=\\n)"), "\n<p>$2</p>");
    string ans = regex_replace(addP, regex(" {4}(.*)"), "$1");
    return ans;
}

vector<string> tableReplaceSplit(string all) {
    regex r1("\\|.*\\|\\n[| :-]+(\\n\\|.*\\|)*");
    regex r2("<figure>[\\s\\S]+?<\\/figure>|[\\s\\S]+?(?=<figure>)|[\\s\\S]+");
    string addTable = regex_replace(all, r1, "<figure><table>$0</table></figure>");
    vector<string> parts = findall(addTable, r2);
    vector<string> ans;
    for (auto &&part : parts) {
        if (part.find("<figure>") == 0) {
            vector<string> lines = split(part, regex("\\n"));
            int columnNumber = std::count(lines[0].begin(), lines[0].end(), '|') - 1;
            vector<string> words = split(lines[1] + lines[0], regex("\\|"));
            vector<string> tableFormat;
            for (int i = 0; i <= columnNumber + 1; i++) {
                if (regex_search(words[i], regex(":-+:")))
                    tableFormat.push_back("<th style='text-align:center;'>");
                else if (regex_search(words[i], regex(":-+")))
                    tableFormat.push_back("<th style='text-align:left;'>");
                else if (regex_search(words[i], regex("-+:")))
                    tableFormat.push_back("<th style='text-align:right;'>");
                else
                    tableFormat.push_back("<th>");
            }

            words = split(lines[0], regex("\\|"));
            string add = "<figure><table>\n<thead><tr>";
            for (int i = 1; i <= columnNumber; i++)
                add += tableFormat[i] + words[i] + "</th>";
            add += "</tr></thead>\n<tbody>";

            for (long long unsigned j = 2; j <= lines.size() - 1; j++) {
                words = split(lines[j] + lines[0], regex("\\|"));
                add += "<tr>";
                for (int i = 1; i <= columnNumber; i++)
                    add += tableFormat[i] + words[i] + "</th>";
                add += "</tr>\n";
            }
            add += "</tbody></table></figure>";
            ans.push_back(add);
        } else
            ans.push_back(part);
    }

    return ans;
}
