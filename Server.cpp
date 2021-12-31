
#include "Server.h"

string socketIO::read(){
    char c=0;
    string s = "";
    while(c != '\n'){
        recv(clientID,&c,sizeof(char),0);
        s += c;
    }
    return s;
}
void socketIO::write(string text){
    const char* txt=text.c_str();
    send(clientID,txt,strlen(txt),0);
}

void socketIO::write(float f){
    ostringstream oss;
    oss <<f;
    string str(oss.str());
    write(str);
}

void socketIO::read(float* f){
}


Server::Server(int port)throw (const char*) {
    socNum = socket(AF_INET,SOCK_STREAM,0);
    if(socNum < 0)
        throw "Error! socket failed";
    hasStopped = false;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(socNum,(struct sockaddr*)&server, sizeof(server))<0)
        throw "bind failure";

    if(listen(socNum, 3)<0)
        throw "listen failure";
}
//
void sigHandler(int sigNum){
    cout<<"sidH"<<endl;
}

void Server::start(ClientHandler& cltHandler)throw(const char*){
    this->trd = new thread([&cltHandler,this](){
        signal(SIGALRM,sigHandler);
        while(this->hasStopped == false){
            socklen_t clientSize = sizeof(this->client);
            alarm(1);
            int acceptedClient = accept(this->socNum,(struct sockaddr*)&this->client,&clientSize);
            if(acceptedClient>0){
                cltHandler.handle(acceptedClient);
                close(acceptedClient);
            }
            alarm(0);

        }
        close(this->socNum);
    });
}

void Server::stopServer(){
    this->hasStopped=true;
    this->trd->join();
}

Server::~Server() {
}
