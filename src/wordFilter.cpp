#include "filter.h"

string wordFilter(string line) {
    string ans1 = linkReplace(imageReplace(line));
    string ans2 = formatReplace(ans1);
    return ans2;
}

string imageReplace(string line) {
    // re.sub(r'!\[(.*?)\]\((.+?)\)', r'<img src="\2" referrerpolicy="no-referrer" alt="\1">', line)
    string ans = regex_replace(line, regex("!\\[(.*?)\\]\\((.+?)\\)"), "<img src='$2' referrerpolicy='no-referrer' alt='$1'>");
    return ans;
}

string linkReplace(string line) {
    // re.sub(r'\[(.*?)\]\((.+?)\)', r'<a href="\2">\1</a>', line)
    string ans1 = regex_replace(line, regex("<\\+(https?://.+?)>"), "<a href='$1' target='_blank' class='url'>$1</a>");
    string ans2 = regex_replace(ans1, regex("\\[(.*?)\\]\\((.+?)\\)"), "<a href='$2'>$1</a>");
    return ans2;
}

string formatReplace(string line) {
    // re.sub(r'\*\*(.+?)\*\*', r'<b>\1</b>', line)
    string star3 = regex_replace(line, regex("\\*\\*\\*(.+?)\\*\\*\\*"), "<b><i>$1</i></b>");
    string star2 = regex_replace(star3, regex("\\*\\*(.+?)\\*\\*"), "<b>$1</b>");
    string star1 = regex_replace(star2, regex("\\*(.+?)\\*"), "<i>$1</i>");
    string blank3 = regex_replace(star1, regex("___(.+?)___"), "<b><i>$1</i></b>");
    string blank2 = regex_replace(blank3, regex("__(.+?)__"), "<b>$1</b>");
    string blank1 = regex_replace(blank2, regex("_(.+?)_"), "<i>$1</i>");
    string ans = regex_replace(blank1, regex("~~(.+?)~~"), "<del>$1</del>");
    return ans;
}
