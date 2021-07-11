#include "filter.h"

string wordFilter(string part) {
    string ans1 = linkReplace(imageReplace(part));
    string ans2 = formatReplace(ans1);
    return ans2;
}

string imageReplace(string part) {
    string ans = regex_replace(part, regex("!\\[(.*?)\\]\\((.+?)\\)"), "<img src='$2' referrerpolicy='no-referrer' alt='$1'>");
    return ans;
}

string linkReplace(string part) {
    string ans1 = regex_replace(part, regex("<\\+(https?://.+?)>"), "<a href='$1' target='_blank' class='url'>$1</a>");
    string ans2 = regex_replace(ans1, regex("\\[(.*?)\\]\\((.+?)\\)"), "<a href='$2'>$1</a>");
    return ans2;
}

string formatReplace(string part) {
    string star3 = regex_replace(part, regex("\\*\\*\\*(.+?)\\*\\*\\*"), "<b><i>$1</i></b>");
    string star2 = regex_replace(star3, regex("\\*\\*(.+?)\\*\\*"), "<b>$1</b>");
    string star1 = regex_replace(star2, regex("\\*(.+?)\\*"), "<i>$1</i>");
    string blank3 = regex_replace(star1, regex("___(.+?)___"), "<b><i>$1</i></b>");
    string blank2 = regex_replace(blank3, regex("__(.+?)__"), "<b>$1</b>");
    string blank1 = regex_replace(blank2, regex("_(.+?)_"), "<i>$1</i>");
    string ans = regex_replace(blank1, regex("~~(.+?)~~"), "<del>$1</del>");
    return ans;
}
