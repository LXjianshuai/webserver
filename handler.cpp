#include "server_base.hpp"
#include<fstream>

using namespace std;
using namespace TinyWeb;

template<typename SERVER_TYPE>
void start_server(SERVER_TYPE &server)
{
    server.resource["^/string/?$"]["POST"] = [](ostream &response,Request &request){
        stringstream ss;
        *request.content>>ss.rdbuf();//rdbuf可以理解为返回一个指针，指代这个流
        string content = ss.str();

        response<<"HTTP/1.1 200 OK\r\nContent-Length: "<<content.length()<<"\r\n\r\n"<<content;
    };

    server.resource["^/info/?$"]["GET"] = [](ostream &response,Request &request){
        stringstream content_stream;
        content_stream<<"<h1>Request:</h1>";
        content_stream<<request.method<<" "<<request.path<<"HTTP/"<<request.http_version<<"<br>";
        for(auto &header:request.header){
            content_stream<<header.first<<":"<<header.second<<"<br>";
        }

        content_stream.seekp(0,ios::end);//把指针指向流的最后，下面tellp（）是计算了当前的长度，通过这种方法就能够计算流的长度

        response<<"HTTP/1.1 200 OK\r\nContent-Length:"<<content_stream.tellp()<<"\r\n\r\n"<<content_stream.rdbuf();
    };

    server.resource["^/match/([0-9a-zA-Z]+)/?$"]["GET"] = [](ostream &response,Request &request){
        string number = request.path_match[1];//匹配结果的第一项就是([0-9a-zA-Z]+)
        response<<"HTTP/1.1 200 OK\r\nContent-Length:"<<number.length()<<"\r\n\r\n"<<number;
    };

    //匹配0个或1个/，在匹配任意个字符，相当于是匹配其他所有的请求
    server.default_resource["^/?(.*)$"]["GET"] = [](ostream &response,Request &request){
        string filename = "web/";
        string path = request.path_match[1];

        size_t last_pos = path.rfind(".");//从结尾找.
        size_t current_pos = 0;
        size_t pos;
        while((pos=path.find('.',current_pos))!=string::npos&&pos!=last_pos){//从开头找.。!=npos代表能够从字符串中找到.，相当于是为了滤除..进入上一个目录
            current_pos = pos;
            path.erase(pos,1);
            last_pos--;//last_pos--,仍然指向的是最后一个.，因为前面erase了，所以长度自动减去1
        }

        filename += path;
        //针对路径中没有.的请求，加上index.html进行查看，如果有.就不进行这一步，仅仅是保留了最后一个.
        if(filename.find('.')==string::npos){
            if(filename[filename.length()-1]!='/')
                filename += '/';
            filename += "index.html";
        }

        ifstream ifs;
        ifs.open(filename,ifstream::in);

        if(ifs){
            ifs.seekg(0,ios::end);
            int length = ifs.tellg();
            ifs.seekg(0,ios::beg);
            response<<"HTTP/1.1 200OK\r\nContent-Length:"<<length<<"\r\n\r\n"<<ifs.rdbuf();
        }//如果打开
        else{
            string content = "Counld not open file"+filename;
            response<<"HTTP/1.1 400 Bad Request\r\nContent-Length:"<<content.length()<<"\r\n\r\n"<<content;
        }
    };
    
    server.start();
    
}

