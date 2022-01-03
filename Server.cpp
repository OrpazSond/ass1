
#include "Server.h"

Server::Server(int port)throw (const char*) {
    sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0) {
        throw "exception";
    }
    sockServer.sin_port = port;
    sockServer.sin_family = AF_INET;
    sockServer.sin_addr.s_addr = INADDR_ANY;

    int b = bind(sock, (struct sockaddr*)&sockServer, sizeof sockServer);
    int l = listen(sock, 5);
    if (b | l < 0){
        throw "exception";
    }
    this->flag = false;
}

void Server::start(ClientHandler& ch)throw(const char*){


}

void Server::stop(){
    t->join(); // do not delete this!
}

Server::~Server() {
}
