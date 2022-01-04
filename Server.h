//Orpaz Sondhelm 206492324 Yarin Tzdaka 319091278


#ifndef SERVER_H_
#define SERVER_H_

#include "commands.h"
#include "CLI.h"

#include <netinet/in.h>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>


using namespace std;



class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};



// you can add helper classes here and implement on the cpp file
class socketIO:public DefaultIO{
    int cltID;
public:
    socketIO(int cltID):cltID(cltID){}
    virtual string read();
    virtual void read(float* f);
    virtual void write(string text);
    virtual void write(float f);

};

// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        socketIO sock(clientID);
        CLI cli(&sock);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
    thread* t; // the thread to run the start() method in
    int sock;
    sockaddr_in sockServer;
    sockaddr_in sockClient;
    bool stopFlag;

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */