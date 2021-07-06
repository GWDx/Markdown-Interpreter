#include "filter.h"

string wordFilter(string line) {
    string temp = linkReplace(imageReplace(line));
    string ans = italicReplace(boldReplace(temp));
    return ans;
}

string imageReplace(string line) {
    // re.sub(r'!\[(.*?)\]\((.+?)\)', r'<img src="\2" referrerpolicy="no-referrer" alt="\1">', line)
    regex r1("!\\[(.*?)\\]\\((.+?)\\)");
    string ans = regex_replace(line, r1, "<img src='$2' referrerpolicy='no-referrer' alt='$1'>");
    return ans;
}

string linkReplace(string line) {
    // re.sub(r'\[(.*?)\]\((.+?)\)', r'<a href="\2">\1</a>', line)
    regex r1("<(https?://.+?)>");
    regex r2("\\[(.*?)\\]\\((.+?)\\)");
    string ans1 = regex_replace(line, r1, "<a href='$1' target='_blank' class='url'>$1</a>");
    string ans2 = regex_replace(ans1, r2, "<a href='$2'>$1</a>");
    return ans2;
}

string boldReplace(string line) {
    // re.sub(r'\*\*(.+?)\*\*', r'<b>\1</b>', line)
    regex r1("\\*\\*(.+?)\\*\\*");
    string ans = regex_replace(line, r1, "<b>$1</b>");
    return ans;
}

string italicReplace(string line) {
    // re.sub(r'\*(.+?)\*', r'<i>\1</i>', line)
    regex r1("\\*(.+?)\\*");
    string ans = regex_replace(line, r1, "<i>$1</i>");
    return ans;
}
