#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<boost/regex.hpp>
// #include<regex>

using std::string;
using std::cout;
using boost::regex;
using boost::regex_replace;

string urlReplace(string line){
    // re.sub(r'\[(.*?)\]\((.+?)\)', r'<a href="\2">\1</a>', line)
    regex r1("\\[(.*?)\\]\\((.+?)\\)");
    string ans = regex_replace(line,r1,"<a href='\\2'>\\1</a>");
    return ans;
}

string listReplace(string all){
    // addUL = re.sub(r'((^|\n)\s*?(\+|-) .*)+', r'\n<ul>\n\0\n</ul>', all)
    // ans = re.sub(r'(^|\n)\s*?> (.*)', r'\n<p>\2</p>', addBlockquote)
    regex r1("((^|\\n)\\s*?(\\+|-) .*)+");
    return all;
}

void segmentFilter(string all){
    string replaced = listReplace(all);
}

int main(){
    std::ifstream inFile;
    std::stringstream buffer;
    inFile.open("test.md");
    buffer << inFile.rdbuf();
    inFile.close();
    string raw = buffer.str();

    // segmentFliter(raw);

    string test("12[abc](de)fg");
    cout<<urlReplace(test);

    string out("1");
    std::ofstream outFile("test.html");
    // outFile.open("test.html");
    outFile << out;
    outFile.close();
    return 0;
}
