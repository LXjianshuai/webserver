#ifndef SERVER_HTTPS
#define SERVER_HTTPS


#include "server_http.hpp"
#include <boost/asio/ssl.hpp>

namespace TinyWeb{
    typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> HTTPS;

    template<>
    class Server<HTTPS>:public ServerBase<HTTPS>{
    public:
        Server(unsigned short port,int number_threads,const string &cert_file,const string &private_key_file):
               ServerBase<HTTPS>::ServerBase(port,number_threads),context(boost::asio::ssl::context::sslv23){
            context.use_certificate_chain_file(cert_file);
            context.use_private_key_file(private_key_file,boost::asio::ssl::context::pem);
        }
    private:
        boost::asio::ssl::context context;
        void accept(){
            auto socket = make_shared<HTTPS>(service,context);
            acceptor.async_accept((*socket).lowest_layer(),
                [this,socket](const boost::system::error_code &ec){
                    accept();
                    if(!ec){
                        (*socket).async_handshake(boost::asio::ssl::stream_base::server,
                                [this,socket](const boost::system::error_code &ec){
                                    if(!ec) process_request_and_respond(socket);
                                });
                    }
                });
        }
    };
};

#endif 

