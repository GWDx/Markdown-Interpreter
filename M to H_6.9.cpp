/*
File: Markdown to HTML
Author: PB19030888-张舒恒
Time：2021/6/9
Function: 标题,分割线,列表,修饰,图片,链接,锚点,引用,代码,表格,目录
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

    bool needMathjax = 0;//latex公式，还未实现

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
//获取文件名
void transform::setTitle(std::string title){
    ContentTitle = title;
}
//当前时间
std::string transform::getTime(){
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}
//运行
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
//读入
int transform::getContent(std::vector<std::string>& content){
    this->content = content;
    contentLength = content.size();
    return 0;
}
//标准输出
void transform::printContent(){
    for(auto v : output){ //auto自适应类型
        std::cout<<v<<std::endl;
    }
}
//输出Markdown
void transform::outputMarkdown(std::vector<std::string> & dests){
    for(auto v : output){
        dests.push_back(v);
    }
}
//转换的主体部分
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
//判断标题
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
//处理标题
std::string transform::handleTitle(std::string s, int titleLevel){
    auto name = s.substr(titleLevel + 1);   //切除##......获取标题名
    auto tem = "<a name=\""+ name +"\"></a>";   //设置锚点
    tem += "<h"+std::to_string(titleLevel)+">"+  name +"</h"+std::to_string(titleLevel)+">"; //把int转成string
    Tocs.push_back(Toc{titleLevel,name});   //标题记录到目录向量里面
    return tem;
}
//判断分割线
int transform::isHorizon(std::string s){
    std::regex e1("^[-*]{3,}$");  //-或*匹配3次以上
    std::smatch sm;         //记录匹配结果
    std::regex_match (s,sm,e1);
    if(sm.size() != 0){
        return 1;
    }else{
        return 0;
    }
}
//处理分割线
std::string transform::handleHorizon(){
    return "<hr>";
}
//判断无序列表
int transform::isUnList(std::string s){
    if(s.empty()){    //判空
        return 0;
    }
    auto idx = 0;
    while(s[idx] == ' ') ++idx;
    if(s[idx] == '+' || s[idx] == '-' || s[idx] == '*'){
        ++idx;
        if(idx < s.size() && s[idx] == ' '){  //后面必须是一个空格
            return idx;                        //返回列表名开始的位置
        }
    }
    return 0;
}
//处理无序列表
void transform::handleUnList(){
    auto ii = 0,top = 0;
    std::stack<int> stack;
    stack.push(0);
    std::string temp;
    for(auto i = 0; i < content.size();++i){
        top = stack.top();
        std::string s = content[i];
        ii = isUnList(s); //判断无序列表
        if(ii <= 0){   //不是列表
            temp = "";
            while(ii < top){ //所有元素出栈
                temp += "</ul>";
                stack.pop();
                top = stack.top();
            }
            temp += s;
            content[i] = temp;
            continue;
        }
        if(ii > stack.top()){ //开启下一级列表
            temp = "<ul><li>" + s.substr(ii + 1) + "</li>";
            stack.push(ii);
        }else if(ii == stack.top()){ //继续当前级别的列表
            temp = "<li>" + s.substr(ii + 1) + "</li>";
        }else{  //是列表但是比当前级别的列表低
            temp = "";
            while(ii < top){  //退出高级别列表
                temp += "</ul>";
                stack.pop();
                top = stack.top();
            }
            if(ii > stack.top()){ //开启下一级列表
                temp += "<ul><li>" + s.substr(ii + 1) + "</li>";
                stack.push(ii);
            }else if(ii == stack.top()){  //继续当前级别的列表
                temp += "<li>" + s.substr(ii + 1) + "</li>";
            }
        }
        content[i] = temp;
    }
}
//有序列表
void transform::handleList(){  //同理无序列表
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
//判断有序列表
int transform::isList(std::string s){
    std::regex e("^\\s*\\d+\\.\\s.+");//\s空格，\d一个数字，+匹配一次及以上，*匹配0次及以上，.任意字符
    std::smatch sm;
    std::regex_match (s,sm,e);
    if(sm.size() != 0){
        auto idx = 0;
        while(s[idx] == ' ') ++idx;
        while(isdigit(s[idx])) ++idx; //isdigit判断数字
        return ++idx;  //返回列表名开始位置
    }else{
        return 0;
    }
}

//判断引用
int transform::isBlock(std::string s){
    std::regex e1("^>*\\s+.*");//匹配>和空格
    std::smatch sm;
    std::regex_match (s,sm,e1);
    if(sm.size()  != 0){
        auto idx = 0;
        while(s[idx] == '>') idx++;
        return idx;   //返回几级引用
    }else{
        return 0;
    }
}

//处理引用
int transform::handleBlock(std::string s){

    int CorNumber = isBlock(s);
    std::string result = "";

    if(status == NORMAL && CorNumber >= 1){
        status = INBLOCK;    //进入INBLOCK模式
        while(BlockNumber < CorNumber){ //BlockNumber记录当前是几级引用
            BlockNumber ++;
            result += "<blockquote>";
        }
        result += s.substr(CorNumber); //去除前面的>
        result += "<br />";//加入换行
        output.push_back(result);
        return 1;
    }else if(status == INBLOCK){
        if(CorNumber > BlockNumber){
            while(BlockNumber < CorNumber){
                BlockNumber ++;
                result += "<blockquote>";
            }
        }else if(CorNumber < BlockNumber){
            while(BlockNumber > CorNumber){ //退出当前级别引用
                BlockNumber --;
                result += "</blockquote>";
            }
        }
        result += s.substr(CorNumber); //去除前面的>
        result += "<br />"; //加入换行
        if(BlockNumber <= 0){  //引用结束，返回NORMAL状态
            status = NORMAL;
            BlockNumber = 0;
        }
        output.push_back(result);
        return 1;
    }
    return 0;
}

//一些出现的字符替换
void transform::handletoken(){
    std::string temp,temp2;
    std::regex re_lt("<");
    std::regex re_and("&");
    std::regex re_link("\\[(.*)\\]\\((.*)\\)");//[]()
    std::regex re_sup("\\[(.*)\\]\\[(.*)\\]");//[][]角标
    std::regex re_img("!\\[(.*)\\]\\((.*)\\)");//![]()
    std::regex re_i("\\*([^\\*]+)\\*");//* *斜体
    std::regex re_S("\\~\\~([^\\~\\~]+)\\~\\~");//删除线
    std::regex re_b("\\*\\*([^\\*\\*]+)\\*\\*");//** **加粗
    std::regex re_m("\\$([^\\$]*)\\$");//latex公式
    std::regex re_code("```(.*)```");//代码
    std::smatch sm;
    for(int i = 0; i < content.size(); ++i){
        temp = content[i];
        temp.insert(temp.begin(), ' ');//最前插入一个空格
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_and,"&amp;"); //std::back_inserter可以动态扩容
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
        temp2.erase(temp2.begin()); //删除最前面的空格
        content[i] = temp2;
    }
}

//判断换行
int transform::isBr(std::string s){
    std::regex re_br("^[\\s\\t]*$"); //匹配空格或者tab
    std::smatch sm;
    std::regex_match(s,sm,re_br);
    if(sm.size() > 0){
        return 1;
    }else{
        return 0;
    }
}


//代码
int transform::handleCode(std::string s){
    std::regex re_Code("^\\s*```\\s*[a-zA-Z1-9\\+\\.]*\\s*$");
    std::regex re_s("\\s");
    std::regex re_t("\\t");
    std::smatch sm;
    std::regex_match(s, sm, re_Code);
    std::string temp,temp2;
    if(status == NORMAL && sm.size() > 0){  //status记录状态跳转
        status = INCODE;                    //进入INCODE状态
        output.push_back("<code> <pre>");
        return 1;
    }else if(status == INCODE && sm.size() > 0){
        status = NORMAL;                    //出INCODE状态，回NORMAL状态
        output.push_back("</pre></code>");
        return 1;
    }else if(status == INCODE){
        std::regex_replace(std::back_inserter(temp),s.begin(),s.end(),re_s,"&nbsp"); //替换空格
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_t,"&nbsp&nbsp&nbsp&nbsp"); //替换tab
        temp="<span>"+temp2+"</span><br />";//span是用来生成样式的，Markdown似乎没有这个功能，所以style暂且没写
        output.push_back(temp);
        return 1;
    }else{
        return 0;
    }
}
//表格
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
//判断目录
int transform::isToc(std::string s){
    std::regex re_toc("\\[TOC\\]"); //[需要转义
    std::smatch sm;
    std::regex_search(s,sm,re_toc);
    if(sm.size() > 0){
        needToc = 1;
        TocContent = s;
        return 1;
    }
    return 0;
}
//处理目录
void transform::handleToc(){
    auto deep = 0;
    auto TocPosition = std::find(output.begin(),output.end(),TocContent); //搜索TOC位置
    if(TocPosition == output.end()){ //未找到
        return;
    }
    for(auto list : Tocs){  //相当于栈，即n级标题后于n-1级标题入栈，n级标题先于n-1级标题出栈，
        if(list.deep > deep){
            while(deep < list.deep){ //deep是当前处于几级标题
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
    while(deep > 0){  //栈中还有元素的话全出栈
        TocString.push_back("</ol>");
        deep --;
    }
    for(int ii = static_cast<int>(TocString.size()) - 1; ii >= 0; ii--){ //倒序插入
        output.insert(TocPosition + 1, TocString[ii]);
    }
}

//流程图
void transform::handleFlow(){

}

int main() {
    std::unique_ptr<transform> transform_init(new transform);//采用std::unique_ptr防止内存泄漏
    std::vector<std::string> cmds;
    std::vector<std::string> c;
    std::vector<std::string> outputs;
    std::string buff;
    std::string from = "test.md";
    std::string dest = "out.html";
    std::ifstream file;
    file.open(from);   //打开文件
    if(!file){
        std::cout<<"Error file\t" + from<<std::endl;
        return -1;
    }
    while(getline(file,buff)){  //读入文件
        c.push_back(buff);
    }
    c.push_back("");
    transform_init->setTitle(from); //获取文件名
    transform_init->getContent(c);
    transform_init->run(c);
    transform_init->outputMarkdown(outputs);

    std::ofstream out; //输出文件
    out.open(dest);
    for(auto s:outputs){
        out<<s<<std::endl;
    }
    return 0;
}
