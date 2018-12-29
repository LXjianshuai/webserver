#include"server_http.hpp"
#include"handler.cpp"
using namespace TinyWeb;

int main()
{
    Server<HTTP> server(12345,4);
    start_server<Server<HTTP>>(server);

    return 0;
}
