#include "filter.h"

string blockFilter(string all) {
    string removeBlank4 = all;
    if (!regex_search(all, regex("<(ul|ol|blockquote|li)"))) {  // 进入标签最内部后再进行替换
        string replaced = quoteReplaceOnce(orderedListReplaceOnce(unorderedListReplaceOnce(all)));
        removeBlank4 = regex_replace(replaced, regex(" {4}(.*)"), "$1");
    }
    regex r1("<(ul|ol|blockquote|li)[\\s\\S]+?<\\/\\1>|.+");
    vector<string> lines = regexFindall(removeBlank4, r1);      // 切分为 <ul>.*</ul> 等标签及普通文本
    regex r2("<((ul|ol|blockquote|li).*?)>([\\s\\S]*)<\\/\\2>");

    string add;
    string ans = "";
    smatch regexResult;
    for (auto &&line : lines) {
        if (regex_match(line, regexResult, r2)) {
            string label1 = regexResult[1];
            string label2 = regexResult[2];
            string rawContent = regexResult[3];
            string content = blockFilter("\n" + rawContent + "\n\n\n");     // 对标签，递归其内部内容
            add = "<+" + label1 + ">" + content + "<+/" + label2 + ">";
        } else
            add = lineFilter(line);                                         // 对普通文本，考虑标题及 <p>
        ans += add + "\n";
    }
    return ans;
}

string unorderedListReplaceOnce(string all) {
    regex r1("\\n {0,3}[*+-] (.*(\\n{1,2}  .*)*)(\\n{1,2} {0,3}[*+-] (.*(\\n{1,2}  .*)*))*");
    regex r2("\\n {0,3}[*+-] (.*(\\n{1,2}  .*)*)");
    string addUL = regex_replace(all, r1, "\n<ul>\n$0\n</ul>");
    string addLI = regex_replace(addUL, r2, "\n<li>$1</li>");
    return addLI;
}

string orderedListReplaceOnce(string all) {
    regex r1("\\n {0,3}(\\d+)\\. (.*(\\n{1,2}  .*)*)(\\n{1,2} {0,3}\\d+\\. (.*(\\n{1,2}  .*)*))*");
    regex r2("\\n {0,3}\\d+\\. (.*(\\n{1,2}  .*)*)");
    string addOL = regex_replace(all, r1, "\n<ol start='$1'>\n$0\n</ol>");
    string addLI = regex_replace(addOL, r2, "\n<li>$1</li>");
    return addLI;
}

string quoteReplaceOnce(string all) {
    string addBlockquote = regex_replace(all, regex("(\\n {0,3}>.*)+"), "\n<blockquote>$0\n</blockquote>");
    string removeMark = regex_replace(addBlockquote, regex("\\n {0,3}>(.*)"), "\n$1");
    return removeMark;
}
