/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include "CLI.h"

#include <iostream>
#include <pthread.h>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<signal.h>
#include <sstream>


using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};

// you can add helper classes

class socketIO:public DefaultIO{
    int clientID;
public:
    socketIO(int clientID):clientID(clientID){}
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        socketIO socketIO(clientID);
        CLI cli(&socketIO);
        cli.start();
    }
};

class Server {
    thread* trd;
    int socNum;
    sockaddr_in server;
    sockaddr_in client;
    volatile bool hasStopped;

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& cltHandler)throw(const char*);
    void stopServer();
};

#endif /* SERVER_H_ */