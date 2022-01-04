//Orpaz Sondhelm 206492324 Yarin Tzdaka 319091278

#include "Server.h"

Server::Server(int port)throw (const char*) {
    sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock < 0) {
        throw "exception";
    }
    sockServer.sin_port = htons(port);
    sockServer.sin_family = AF_INET;
    sockServer.sin_addr.s_addr = INADDR_ANY;
    int b = bind(sock, (struct sockaddr*)&sockServer, sizeof sockServer);
    int l = listen(sock, 4);
    if (b | l < 0){
        throw "exception";
    }
    this->stopFlag = false;
}


string socketIO::read(){
    string inputString = "";
    char readChar;
    while (readChar != '\n'){
        recv(this->cltID, &readChar, sizeof(char), 0);
        inputString = inputString + readChar;
    }
    return inputString;
}
void socketIO::write(string text) {
    send(this->cltID, text.c_str(), text.size(), 0);
}

void socketIO::write(float f){
    char arr[20];
    gcvt(f,20,arr);
    string str(arr);
    write(str);
}

void socketIO::read(float* f){
}


void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch, this](){
        while(stopFlag == false){
            socklen_t cltSize = sizeof(sockClient);
            struct sockaddr* address = (struct sockaddr*)&sockClient;
            int acceptedClient = accept(sock,address,&cltSize);
            if(acceptedClient > 0) {
                ch.handle(acceptedClient);
                close(acceptedClient);
            }
        }
        close(sock);
    });

}

void Server::stop(){
    stopFlag = true;
    t->join(); // do not delete this!
}

Server::~Server() {
}
