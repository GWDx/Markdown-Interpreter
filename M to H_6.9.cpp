/*
File: Markdown to HTML
Author: PB19030888-����� 
Time��2021/6/9 
Function: ����,�ָ���,�б�,����,ͼƬ,����,ê��,����,����,���,Ŀ¼ 
Compiler: gcc 4.9.2
Compile Order: -std=c++11
*/ 
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <regex>
#include <time.h>

class transform{
public:
    void outputMarkdown(std::vector<std::string> & dests);
    void setTitle(std::string title);
    
    int run(std::vector<std::string>& content);
    int getContent(std::vector<std::string>& content);
    void printContent();
    int translate();
private:
    std::string ContentTitle;
    
    enum All_Statue{
        INCODE,
        NORMAL,
        INBLOCK,
        INTABLE,
        TABLE_FMT
    } status;
    
    std::string preReplace(std::string s);
    std::string getTime();
    
    std::vector<std::string> content;
    size_t contentLength;
    std::vector<std::string> output;
    
    int isToc(std::string s);
    std::string TocContent;
    struct Toc{int deep; std::string name;};
    bool needToc = 0;
    int TocNumber = 0;
    std::vector<struct Toc> Tocs;
    std::vector<std::string> TocString;
    void handleToc();
    
    bool needMathjax = 0;//latex��ʽ����δʵ�� 
    
    int isTitle(std::string s);
    int isHorizon(std::string s);
    
    std::string handleTitle(std::string s,int titleLevel);
    std::string handleHorizon();
    
    void handleUnList();
    int isUnList(std::string s);
    
    void handleList();
    int isList(std::string s);
    
    int BlockNumber = 0;
    int isBlock(std::string s);
    
    int handleBlock(std::string s);
    
    int isBr(std::string s);
    
    int isCode(std::string s);
    void handletoken();
    
    int handleCode(std::string s);
    
    int TableNumber = 0;
    std::vector<std::string> aligns;
    int handleTable(std::string s);
    std::vector<std::string> getTableElem(std::string s);
    
    void handleFlow();
};
//��ȡ�ļ��� 
void transform::setTitle(std::string title){
    ContentTitle = title;
}
//��ǰʱ�� 
std::string transform::getTime(){
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}
//���� 
int transform::run(std::vector<std::string>& content){
    for(int i = 0;i < 10;++i){
        aligns.push_back("center");
    }
    status = NORMAL;
    BlockNumber = 0;
    TableNumber = 0;
    output.push_back("<!DOCTYPE html>");
    output.push_back("<html>");
    output.push_back("<head>");
    output.push_back("<title>" + ContentTitle + "</title>");
    output.push_back("<meta name=\"Time\" content=\""+getTime()+"\" />");
    output.push_back("</style>");
    output.push_back("<meta charset=\"utf-8\"/>");
    output.push_back("</head>");
    output.push_back("<body>");   
    translate();
    if(needToc == true){
        handleToc();
    }
    if(needMathjax == 0){
        std::vector<std::string>::iterator it = output.begin() + 4;
        output.erase(it);
    }
    output.push_back("</body>");
    output.push_back("</html>");
    return 0;
}
//���� 
int transform::getContent(std::vector<std::string>& content){
    this->content = content;
    contentLength = content.size();
    return 0;
}
//��׼��� 
void transform::printContent(){
    for(auto v : output){ //auto����Ӧ���� 
        std::cout<<v<<std::endl;
    }
}
//���Markdown
void transform::outputMarkdown(std::vector<std::string> & dests){
    for(auto v : output){
        dests.push_back(v);
    }
}
//ת�������岿�� 
int transform::translate(){
    handletoken();
    handleUnList();
    handleList();
    handleFlow(); 
    for(auto s : content){
        if(needToc == 0) isToc(s);
        auto titleLevel = isTitle(s);
        
        if(handleBlock(s) == 1){}
        else if(handleCode(s) == 1){}
        else if(handleTable(s) == 1){}
        else if(titleLevel != 0){
            output.push_back(handleTitle(s, titleLevel));
        }else if(isHorizon(s)){
            output.push_back(handleHorizon());
        
        }else if(isBr(s)){
            output.push_back("<br />");
        }else{
            output.push_back(s);
        }
    }
    return 0;
}
//�жϱ��� 
int transform::isTitle(std::string s){
    auto titleLevel = 0;
    if(s.empty()){
        return 0;
    }
    while(s[titleLevel] == '#'){
        ++titleLevel;
        if(titleLevel > 7) break;
    }
    if(titleLevel > 7 || s[titleLevel] != ' '){
        titleLevel = 0;
    }
    return titleLevel;
}
//�������
std::string transform::handleTitle(std::string s, int titleLevel){
    auto name = s.substr(titleLevel + 1);   //�г�##......��ȡ������ 
    auto tem = "<a name=\""+ name +"\"></a>";   //����ê�� 
    tem += "<h"+std::to_string(titleLevel)+">"+  name +"</h"+std::to_string(titleLevel)+">"; //��intת��string 
    Tocs.push_back(Toc{titleLevel,name});   //�����¼��Ŀ¼�������� 
    return tem;
}
//�жϷָ��� 
int transform::isHorizon(std::string s){
    std::regex e1("^[-*]{3,}$");  //-��*ƥ��3������ 
    std::smatch sm;         //��¼ƥ���� 
    std::regex_match (s,sm,e1);
    if(sm.size() != 0){
        return 1;
    }else{
        return 0;
    }
}
//����ָ��� 
std::string transform::handleHorizon(){
    return "<hr>";
}
//�ж������б� 
int transform::isUnList(std::string s){
    if(s.empty()){    //�п� 
        return 0;
    }
    auto idx = 0;
    while(s[idx] == ' ') ++idx;
    if(s[idx] == '+' || s[idx] == '-' || s[idx] == '*'){
        ++idx;
        if(idx < s.size() && s[idx] == ' '){  //���������һ���ո� 
            return idx;                        //�����б�����ʼ��λ�� 
        }
    }
    return 0;
}
//���������б� 
void transform::handleUnList(){
    auto ii = 0,top = 0;
    std::stack<int> stack;
    stack.push(0);
    std::string temp;
    for(auto i = 0; i < content.size();++i){
        top = stack.top();
        std::string s = content[i];
        ii = isUnList(s); //�ж������б�  
        if(ii <= 0){   //�����б� 
            temp = "";
            while(ii < top){ //����Ԫ�س�ջ 
                temp += "</ul>";
                stack.pop();
                top = stack.top();
            }
            temp += s;
            content[i] = temp;
            continue;
        }
        if(ii > stack.top()){ //������һ���б� 
            temp = "<ul><li>" + s.substr(ii + 1) + "</li>";
            stack.push(ii);
        }else if(ii == stack.top()){ //������ǰ������б� 
            temp = "<li>" + s.substr(ii + 1) + "</li>";
        }else{  //���б��Ǳȵ�ǰ������б�� 
            temp = "";
            while(ii < top){  //�˳��߼����б� 
                temp += "</ul>";
                stack.pop();
                top = stack.top();
            }
            if(ii > stack.top()){ //������һ���б�  
                temp += "<ul><li>" + s.substr(ii + 1) + "</li>";
                stack.push(ii);
            }else if(ii == stack.top()){  //������ǰ������б� 
                temp += "<li>" + s.substr(ii + 1) + "</li>";
            }
        }
        content[i] = temp;
    }
}
//�����б� 
void transform::handleList(){  //ͬ�������б� 
    auto ii = 0,top = 0;
    std::stack<int> stack;
    stack.push(0);
    std::string temp;
    for(auto i = 0; i < content.size(); ++i){
        top = stack.top();
        std::string s = content[i];
        ii = isList(s);
        if(ii <= 1){
            temp = "";
            while(ii < top){
                temp += "</ol>";
                stack.pop();
                top = stack.top();
            }
            temp += s;
            content[i] = temp;
            continue;
        }
        if(ii > stack.top()){
            temp = "<ol><li>" + s.substr(ii + 1) + "</li>";
            stack.push(ii);
        }else if(ii == stack.top()){
            temp = "<li>" + s.substr(ii + 1) + "</li>";
        }else{
            temp = "";
            while(ii < top){
                temp += "</ol>";
                stack.pop();
                top = stack.top();
            }
            if(ii > stack.top()){
                temp += "<ol><li>" + s.substr(ii + 1) + "</li>";
                stack.push(ii);
            }else if(ii == stack.top()){
                temp += "<li>" + s.substr(ii + 1) + "</li>";
            }
        }
        content[i] = temp;
    }
}
//�ж������б� 
int transform::isList(std::string s){
    std::regex e("^\\s*\\d+\\.\\s.+");//\s�ո�\dһ�����֣�+ƥ��һ�μ����ϣ�*ƥ��0�μ����ϣ�.�����ַ� 
    std::smatch sm;
    std::regex_match (s,sm,e);
    if(sm.size() != 0){
        auto idx = 0;
        while(s[idx] == ' ') ++idx;
        while(isdigit(s[idx])) ++idx; //isdigit�ж����� 
        return ++idx;  //�����б�����ʼλ�� 
    }else{
        return 0;
    }
}

//�ж����� 
int transform::isBlock(std::string s){
    std::regex e1("^>*\\s+.*");//ƥ��>�Ϳո� 
    std::smatch sm;
    std::regex_match (s,sm,e1);
    if(sm.size()  != 0){
        auto idx = 0;
        while(s[idx] == '>') idx++;
        return idx;   //���ؼ�������
    }else{
        return 0;
    }
}

//�������� 
int transform::handleBlock(std::string s){
    
    int CorNumber = isBlock(s);
    std::string result = "";
    
    if(status == NORMAL && CorNumber >= 1){
        status = INBLOCK;    //����INBLOCKģʽ 
        while(BlockNumber < CorNumber){ //BlockNumber��¼��ǰ�Ǽ������� 
            BlockNumber ++;
            result += "<blockquote>";
        }
        result += s.substr(CorNumber); //ȥ��ǰ���> 
        result += "<br />";//���뻻�� 
        output.push_back(result);
        return 1;
    }else if(status == INBLOCK){
        if(CorNumber > BlockNumber){
            while(BlockNumber < CorNumber){
                BlockNumber ++;
                result += "<blockquote>";
            }
        }else if(CorNumber < BlockNumber){
            while(BlockNumber > CorNumber){ //�˳���ǰ�������� 
                BlockNumber --;
                result += "</blockquote>";
            }
        }
        result += s.substr(CorNumber); //ȥ��ǰ���>
        result += "<br />"; //���뻻�� 
        if(BlockNumber <= 0){  //���ý���������NORMAL״̬ 
            status = NORMAL;
            BlockNumber = 0;
        }
        output.push_back(result);
        return 1;
    }
    return 0;
}

//һЩ���ֵ��ַ��滻 
void transform::handletoken(){
    std::string temp,temp2;
    std::regex re_lt("<");
    std::regex re_and("&");
    std::regex re_link("\\[(.*)\\]\\((.*)\\)");//[]()
    std::regex re_sup("\\[(.*)\\]\\[(.*)\\]");//[][]�Ǳ� 
    std::regex re_img("!\\[(.*)\\]\\((.*)\\)");//![]()
    std::regex re_i("\\*([^\\*]+)\\*");//* *б�� 
    std::regex re_S("\\~\\~([^\\~\\~]+)\\~\\~");//ɾ���� 
    std::regex re_b("\\*\\*([^\\*\\*]+)\\*\\*");//** **�Ӵ� 
    std::regex re_m("\\$([^\\$]*)\\$");//latex��ʽ 
    std::regex re_code("```(.*)```");//���� 
    std::smatch sm;
    for(int i = 0; i < content.size(); ++i){
        temp = content[i];
        temp.insert(temp.begin(), ' ');//��ǰ����һ���ո� 
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_and,"&amp;"); //std::back_inserter���Զ�̬���� 
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_lt,"&lt;");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),std::regex("\\\\&amp;"),"&");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),std::regex("\\\\&lt;"),"<");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_img,"<img src=\"$2\" align=\"middle\">");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_sup,"$1<sup>$2</sup>");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_link,"<a href=\"$2\" target=\"_blank\">$1</a>");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_b,"<b> $1 </b>");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_S,"<s> $1 </s>");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_i,"<i> $1 </i>");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_code,"<code>  $1 </code>");
        std::regex_match(temp,sm,re_m);
        if(sm.size() > 0){
            needMathjax = 1;
        }
        temp2.erase(temp2.begin()); //ɾ����ǰ��Ŀո� 
        content[i] = temp2;
    }
}

//�жϻ��� 
int transform::isBr(std::string s){
    std::regex re_br("^[\\s\\t]*$"); //ƥ��ո����tab
    std::smatch sm;
    std::regex_match(s,sm,re_br);
    if(sm.size() > 0){
        return 1;
    }else{
        return 0;
    }
}


//���� 
int transform::handleCode(std::string s){
    std::regex re_Code("^\\s*```\\s*[a-zA-Z1-9\\+\\.]*\\s*$");
    std::regex re_s("\\s");
    std::regex re_t("\\t");
    std::smatch sm;
    std::regex_match(s, sm, re_Code);
    std::string temp,temp2;
    if(status == NORMAL && sm.size() > 0){  //status��¼״̬��ת 
        status = INCODE;                    //����INCODE״̬ 
        output.push_back("<code> <pre>");
        return 1;
    }else if(status == INCODE && sm.size() > 0){
        status = NORMAL;                    //��INCODE״̬����NORMAL״̬ 
        output.push_back("</pre></code>");
        return 1;
    }else if(status == INCODE){
        std::regex_replace(std::back_inserter(temp),s.begin(),s.end(),re_s,"&nbsp"); //�滻�ո� 
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_t,"&nbsp&nbsp&nbsp&nbsp"); //�滻tab 
        temp="<span>"+temp2+"</span><br />";//span������������ʽ�ģ�Markdown�ƺ�û��������ܣ�����style����ûд 
        output.push_back(temp);
        return 1;
    }else{
        return 0;
    }
}
//��� 
int transform::handleTable(std::string s){
    std::regex re_Table("\\|(.*)\\|");
    std::regex re_rep("\\|");
    std::smatch sm;
    std::regex_match(s,sm,re_Table);
    std::string result = "";
    std::vector<std::string> elem;
    if(status == NORMAL && sm.size() > 0){
        elem = getTableElem(s);
        result += "<table border=\"1\" align=\"left\" cellspacing=\"1\" cellpadding=\"5\">";
        result += "<tr>";
        for(auto e : elem){
            result += ("<th>" +  e + "</th>");
        }
        result += "</tr>";
        TableNumber = static_cast<int>(elem.size());
        status = TABLE_FMT;
        output.push_back(result);
        return 1;
    }else if(status == TABLE_FMT && sm.size() > 0){
        elem = getTableElem(s);
        for(auto ii = 0; ii < TableNumber; ++ii){
            std::regex_match(elem[ii],sm,std::regex("\\:\\-*"));
            if(sm.size() > 0) aligns[ii] = "left";
            std::regex_match(elem[ii],sm,std::regex("\\:\\-*\\:"));
            if(sm.size() > 0) aligns[ii] = "center";
            std::regex_match(elem[ii],sm,std::regex("\\-*\\:"));
            if(sm.size() > 0) aligns[ii] = "right" ;
        }
        status = INTABLE;
        return 1;
    }else if(status == INTABLE && sm.size() > 0){
        elem = getTableElem(s);
        result += "<tr>";
        for(auto ii = 0; ii < TableNumber; ++ii){
            result += ("<td align=\""+aligns[ii]+"\">" +elem[ii]+ "</td>");
        }
        result += "</tr>";
        output.push_back(result);
        return 1;
    }else if((status == INTABLE || status == TABLE_FMT) && sm.size() == 0){
        TableNumber = 0;
        status = NORMAL;
        result = "</table>"+ s;
        output.push_back(result);
        return 1;
    }
    return 0;
}

std::vector<std::string> transform::getTableElem(std::string s){
    std::vector<std::string> result;
    std::string temp = "";
    for(auto c : s){
        if(c == '|'){
            result.push_back(temp);
            temp.clear();
        }else{
            temp += c;
        }
    }
    result.erase(result.begin());
    return result;
}
//�ж�Ŀ¼ 
int transform::isToc(std::string s){
    std::regex re_toc("\\[TOC\\]"); //[��Ҫת�� 
    std::smatch sm;
    std::regex_search(s,sm,re_toc);
    if(sm.size() > 0){
        needToc = 1;
        TocContent = s;
        return 1;
    }
    return 0;
}
//����Ŀ¼ 
void transform::handleToc(){
    auto deep = 0;
    auto TocPosition = std::find(output.begin(),output.end(),TocContent); //����TOCλ�� 
    if(TocPosition == output.end()){ //δ�ҵ� 
        return;
    }
    for(auto list : Tocs){  //�൱��ջ����n���������n-1��������ջ��n����������n-1�������ջ�� 
        if(list.deep > deep){
            while(deep < list.deep){ //deep�ǵ�ǰ���ڼ������� 
                TocString.push_back("<ol style = \"line-height:80%\">");
                deep ++;
            }
            TocString.push_back("<li> <a href=\"#"+list.name+"\">"+list.name+"</a> </li>");
        }else if(list.deep == deep){
            TocString.push_back("<li> <a href=\"#"+list.name+"\">"+list.name+"</a> </li>");
        }else{
            while(deep > list.deep){
                TocString.push_back("</ol>");
                deep --;
            }
            TocString.push_back("<li> <a href=\"#"+list.name+"\">"+list.name+"</a> </li>");
        }
    }
    while(deep > 0){  //ջ�л���Ԫ�صĻ�ȫ��ջ 
        TocString.push_back("</ol>");
        deep --;
    }
    for(int ii = static_cast<int>(TocString.size()) - 1; ii >= 0; ii--){ //������� 
        output.insert(TocPosition + 1, TocString[ii]);
    }
}

//����ͼ 
void transform::handleFlow(){
  
}

int main() {
    std::unique_ptr<transform> transform_init(new transform);//����std::unique_ptr��ֹ�ڴ�й© 
    std::vector<std::string> cmds;
    std::vector<std::string> c;
    std::vector<std::string> outputs;
    std::string buff;
    std::string from = "test.txt";
    std::string dest = "out.html";
    std::ifstream file;
    file.open(from);   //���ļ� 
    if(!file){
        std::cout<<"Error file\t" + from<<std::endl;
        return -1;
    }
    while(getline(file,buff)){  //�����ļ� 
        c.push_back(buff);
    }
    c.push_back("");
    transform_init->setTitle(from); //��ȡ�ļ��� 
    transform_init->getContent(c);
    transform_init->run(c);
    transform_init->outputMarkdown(outputs);
    
    std::ofstream out; //����ļ� 
    out.open(dest);
    for(auto s:outputs){
        out<<s<<std::endl;
    }
    return 0;
}
