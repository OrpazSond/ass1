/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include "commands.h"
#include "CLI.h"
#include <netinet/in.h>

using namespace std;


// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};



// you can add helper classes here and implement on the cpp file


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
    bool flag;

    // you may add data members

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */