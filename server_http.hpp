#ifndef SERVER_HTTP
#define SERVER_HTTP

#include"server_base.hpp"
using namespace std;
namespace TinyWeb{
    typedef boost::asio::ip::tcp::socket HTTP;

    template<>
    class Server<HTTP>:public ServerBase<HTTP>{
    public:
        Server(unsigned short port,size_t thread_num=1):ServerBase<HTTP>::ServerBase(port,thread_num){};

    private:
        void accept(){
            //socket的类型是shared_ptr<HTTP> 
            auto socket = make_shared<HTTP>(service);

            acceptor.async_accept(*socket,[this,socket](const boost::system::error_code &ec){
                accept();
                if(!ec)
                    process_request_and_respond(socket);
            });
        }
    };
}

#endif 
