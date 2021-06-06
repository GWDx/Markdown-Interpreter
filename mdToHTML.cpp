#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<cstring>
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
    //addUL = re.sub(r'((^|\n)\s*?(\+|-) .*)+', r'\n<ul>\n\0\n</ul>', all)
    //ans = re.sub(r'(^|\n)\s*?(\+|-) (.*)', r'\n<li>\3</li>', addUL)
    regex r1("((^|\\n)\\s*?(\\+|-) .*)+");
    regex r2("(^|\\n)\\s*?(\\+|-) (.*)");
	string addUL = regex_replace(all,r1,"\\n<ul>\\n\\0\\n</ul>");
	string ans = regex_replace(addUL,r2,"\\n<li>\\3</li>");
    return ans;
}

string codeReplace(string all){
	//ans = re.sub(r'\s+```(.*?)\n(.*?)```\n',r'\n<pre><code lang="\1">\2</code></pre>\n', all)
	regex r1("\\s+```(.*?)\\n(.*?)```\\n");
	string ans = regex_replace(all,r1,"\\n<pre><code lang="\\1">\\2</code></pre>\\n");
    return ans;	
}
    
string quoteReplace(string all){
	//addBlockquote = re.sub(r'((^|\n)\s*?> .*)+', r'\n<blockquote>\n\0\n</blockquote>', all)
	//ans = re.sub(r'(^|\n)\s*?> (.*)', r'\n<p>\2</p>', addBlockquote)
	regex r1("((^|\\n)\\s*?> .*)+");
    regex r2("(^|\\n)\\s*?> (.*)");
	string addBlockquote = regex_replace(all,r1,"\\n<blockquote>\\n\\0\\n</blockquote>");
	string ans = regex_replace(addBlockquote,r2,"\\n<p>\\2</p>");
    return ans;
}  
string boldReplace(string line){
	regex r1("\\*\\*(.+?)\\*\\*");
	string ans = regex_replace(line,r1,"<b>\\1</b>");
    return ans;	
}

string bodyAppend(string ans){
	string ans_ = "";
	string bodyFirst = "<body>\n";
	string bodyLast = "</body>\n";	
	ans_.append(bodyFirst);
	ans_.append(ans);
	ans_.append(bodyLast);
	return ans_;
}
    
string htmlAppend(string ans){
	string ans_ = "";
	string htmlFirst = "<!doctype html>\n<html>\n";	
	string htmlLast = "</html>\n";	
	ans_.append(htmlFirst);
	ans_.append(ans);
	ans_.append(htmlLast);
	return ans_;
}
    
string pAppend(string line){
	string ans_ = "";
	string pFirst = "<p>";	
	string pLast = "</p>";	
	ans_.append(pFirst);
	ans_.append(ans);
	ans_.append(pLast);
	return ans_;
}
    
string segmentFliter(string all){
	string replaced = quoteReplace(listReplace(codeReplace(all)))
	regex r1("<(?:pre|ul|blockquote)>[\\s\\S]*?<\\/(?:pre|ul|blockquote)>|\\n|.+");
	string ans = regex_search(replaced,r1);
	string result;
	int i=0;
	while(ans[i]!=EOF){
		if(ans[i]==\n)
			result[i]='';
		else 
		    result[i]=ans[i];
		i++;
	}
	return result;
}
              
string wordFilter(line){	
	return boldReplace(urlReplace(line));	
}
   
string lineFilter(line){
	string head = headReplace(line);
	if(head)
		return head;
	return pAppend(line);	
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
