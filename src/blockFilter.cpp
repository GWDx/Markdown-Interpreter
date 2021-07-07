#include "filter.h"

string blockFilter(string all) { // ? nest
    string code = codeReplace(all);
    string list = orderedListReplace(unorderedListReplace(code));
    string quote = quoteReplace(list);
    return quote;
}

string codeReplace(string all) {
    // ans = re.sub(r'\s+```(.*)([\s\S]*?)\s+```\n', r'\n<pre><code lang="\1">\2</code></pre>\n', all)
    string ans = regex_replace(all, regex("\\s+```(.*)([\\s\\S]*?)\\s+```\\n"), "\n<pre><code lang='$1'>$2</code></pre>\n");
    return ans;
}

string unorderedListReplace(string all) {
    // addUL = re.sub(r'((^|\n)\s*?(\+|-) .*)+', r'\n<ul>\n\0\n</ul>', all)
    // ans = re.sub(r'(^|\n)\s*?(\+|-) (.*)', r'\n<li>\3</li>', addUL)
    string addUL = regex_replace(all, regex("((^|\\n)\\s*?(\\+|-) .*)+"), "\n<ul>\n$0\n</ul>");
    string ans = regex_replace(addUL, regex("(^|\\n)\\s*?(\\+|-) (.*)"), "\n<li>$3</li>");
    return ans;
}

string orderedListReplace(string all) { // ? <ol start='4'
    string addOL = regex_replace(all, regex("((^|\\n)\\s*?\\d .*)+"), "\n<ol>\n$0\n</ol>");
    string ans = regex_replace(addOL, regex("(^|\\n)\\s*?\\d (.*)"), "\n<li>$2</li>");
    return ans;
}

string quoteReplace(string all) {
    // addBlockquote = re.sub(r'((^|\n)\s*?> .*)+', r'\n<blockquote>\n\0\n</blockquote>', all)
    // ans = re.sub(r'(^|\n)\s*?> (.*)', r'\n<p>\2</p>', addBlockquote)
    string addBlockquote = regex_replace(all, regex("((^|\\n)\\s*?> .*)+"), "\n<blockquote>\n$0\n</blockquote>");
    string ans = regex_replace(addBlockquote, regex("(^|\\n)\\s*?> (.*)"), "\n<p>$2</p>");
    return ans;
}
