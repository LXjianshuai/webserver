#include"server_http.hpp"
#include"server_https.hpp"
#include"handler.cpp"
using namespace TinyWeb;

int main()
{
    Server<HTTPS> server(12345,4,"server.crt","server.key");
    start_server<Server<HTTPS>>(server);

    return 0;
}
