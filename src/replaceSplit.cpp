
#include "filter.h"

vector<string> codeReplaceSplit(string all) {
    string ans = regex_replace(all, regex("\\n```(.*)([\\s\\S]*?)```\\n"), "\n<pre><code lang='$1'>$2</code></pre>\n");
    // regex r1("(?=<pre>)|(?<=\\/pre>)");
    // vector<string> codeParts = regexSplit(ans, r1);
    regex r1("<pre>[\\s\\S]*?<\\/pre>|[\\s\\S]+?(?=<pre>)|[\\s\\S]+");
    vector<string> codeParts = regexFindall(ans, r1);
    return codeParts;
}

vector<string> tableReplaceSplit(string all) {
    regex r1("\\|.*\\|\\n[| :-]+(\\n\\|.*\\|)*");
    regex r2("<figure>[\\s\\S]+?<\\/figure>|[\\s\\S]+?(?=<figure>)|[\\s\\S]+");
    string addTable = regex_replace(all, r1, "<figure><table>$0</table></figure>");
    vector<string> tableParts = regexFindall(addTable, r2);
    vector<string> ans;
    for (auto &&tablePart : tableParts) {
        if (tablePart.find("<figure>") == 0) {
            vector<string> lines = regexSplit(tablePart, regex("\\n"));
            int columnNumber = std::count(lines[0].begin(), lines[0].end(), '|') - 1;
            vector<string> words = regexSplit(lines[1] + lines[0], regex("\\|"));
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

            words = regexSplit(lines[0], regex("\\|"));
            string add = "<figure><table>\n<thead><tr>";
            for (int i = 1; i <= columnNumber; i++)
                add += tableFormat[i] + words[i] + "</th>";
            add += "</tr></thead>\n<tbody>";

            for (long long unsigned j = 2; j <= lines.size() - 1; j++) {
                words = regexSplit(lines[j] + lines[0], regex("\\|"));
                add += "<tr>";
                for (int i = 1; i <= columnNumber; i++)
                    add += tableFormat[i] + words[i] + "</th>";
                add += "</tr>\n";
            }
            add += "</tbody></table></figure>";
            ans.push_back(add);
        } else
            ans.push_back(tablePart);
    }

    return ans;
}
