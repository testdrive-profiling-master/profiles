//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : System remote
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include "TDIPC.h"
#include<iostream>
#include<boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

class session
{
public:
    session(tcp::socket& sock)
    : socket_(sock){
        start();
    }

    void start(){
        try{
            while(1){
                boost::system::error_code error;
                size_t length = socket_.read_some(boost::asio::buffer(data_), error);
                if(error == boost::asio::error::eof){
                    // exit out
                    break;
                }else if(error){
                    throw boost::system::system_error(error);
                }
                cout << "Message from client: " << data_ << endl;
                boost::asio::write(socket_, boost::asio::buffer(data_, length));
            }
        }catch(exception& e){
            cerr << "Exception in server: " << e.what() << endl;
        }
    }

    tcp::socket& socket(){return socket_;}

private:
    tcp::socket& socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class server
{
public:
    server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service), acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        while(1){
            tcp::socket sock(io_service);
            //wait for client
            acceptor_.accept(sock);
            new session(sock);
        }
    }
private:
    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};

class client
{
public:
    client(boost::asio::io_service& io_service, const string& host, const string& port) : socket_(io_service)
    {
        tcp::resolver resolver(io_service);
        //try to connect to server
        boost::asio::connect(socket_, resolver.resolve({host, port}));

        string msg = "Hello, world!";
        size_t msg_length = msg.length();

        //send data
        boost::asio::write(socket_, boost::asio::buffer(msg, msg_length));

        char reply[max_length];
        //get data
        size_t reply_length = boost::asio::read(socket_, boost::asio::buffer(reply, msg_length));
        cout << "Reply is: " << reply << endl;

    }

private:
    tcp::socket socket_;
    enum { max_length = 1024 };
};


int main_server(int argc, char* argv[]){
    try{
        if(argc != 2){
            cerr << "Usage: server <port>" << endl;
            return 1;
        }
        boost::asio::io_service io_service;
        server s(io_service, atoi(argv[1]));

    }catch(exception& e){
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}

int main_client(int argc, char* argv[]){
    try{
        if(argc != 3){
            cerr << "Usage: client <host> <port>" << endl;
            return 1;
        }

        boost::asio::io_service io_service;
        client c(io_service, argv[1], argv[2]);

    }catch(exception& e){
        cerr<<"Exception: " << e.what() << endl;
    }

    return 0;
}
