#include "filter.h"

string blockFilter(string all) {    // ? nest
    string code = codeReplace(all);
    string list = orderedListReplace(unorderedListReplace(code));
    string quote = quoteReplace(list);
    return quote;
}

string codeReplace(string all) {
    // ans = re.sub(r'\s+```(.*?)\n(.*?)```\n', r'\n<pre><code lang="\1">\2</code></pre>\n', all, flags=re.DOTALL)
    regex r1("\\s+```(.*)([\\s\\S]*?)```\\n");
    string ans = regex_replace(all, r1, "\n<pre><code lang='$1'>$2</code></pre>\n");
    return ans;
}

string unorderedListReplace(string all) {
    // addUL = re.sub(r'((^|\n)\s*?(\+|-) .*)+', r'\n<ul>\n\0\n</ul>', all)
    // ans = re.sub(r'(^|\n)\s*?(\+|-) (.*)', r'\n<li>\3</li>', addUL)
    regex r1("((^|\\n)\\s*?(\\+|-) .*)+");
    regex r2("(^|\\n)\\s*?(\\+|-) (.*)");
    string addUL = regex_replace(all, r1, "\n<ul>\n$0\n</ul>");
    string ans = regex_replace(addUL, r2, "\n<li>$3</li>");
    return ans;
}

string orderedListReplace(string all) { // <ol start='4'
    regex r1("((^|\\n)\\s*?\\d .*)+");
    regex r2("(^|\\n)\\s*?\\d (.*)");
    string addOL = regex_replace(all, r1, "\n<ol>\n$0\n</ol>");
    string ans = regex_replace(addOL, r2, "\n<li>$2</li>");
    return ans;
}

string quoteReplace(string all) {
    // addBlockquote = re.sub(r'((^|\n)\s*?> .*)+', r'\n<blockquote>\n\0\n</blockquote>', all)
    // ans = re.sub(r'(^|\n)\s*?> (.*)', r'\n<p>\2</p>', addBlockquote)
    regex r1("((^|\\n)\\s*?> .*)+");
    regex r2("(^|\\n)\\s*?> (.*)");
    string addBlockquote = regex_replace(all, r1, "\n<blockquote>\n$0\n</blockquote>");
    string ans = regex_replace(addBlockquote, r2, "\n<p>$2</p>");
    return ans;
}
