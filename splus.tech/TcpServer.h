/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/* 
 * File:   TcpServer.h
 * Author: dies irae
 *
 * Created on July 2, 2020, 4:21 PM
 */
#pragma once
#include "TcpServer.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <limits.h>
#include <vector>
#include <algorithm>
using namespace std;
#pragma comment (lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE (49152)
#ifndef TCPSERVER_H
#define TCPSERVER_H


class TcpServer {
public:
    TcpServer(const char * ipAddress, int port){
        svr_addr = ipAddress;
        svr_port = port;
        svr_socket;
        cl_so_main;
        cl_ip_addr[16] = '\0';
        fd_in = new fd_set;
        fd_out = new fd_set;
        fd_ex = new fd_set;
    }
    int init();
    int run();
    char * get_cl_ip_addrs();
protected:
    virtual void onClientConnected(int clientSocket);
    virtual void onClientDisconnected(int clientSocket);
    virtual void onMessageReceived(int clientSocket, const char* msg, int length);
    void sendToClient(int clientSocket, const char* msg, int length);
    void broadcastToClients(int sendingClient, const char* msg, int length);
private:
    const char *          svr_addr;
    int                   svr_port;
    int                   svr_socket;
    vector<int>           cl_so_main;
    char                  cl_ip_addr[16];
    fd_set                * fd_in;
    fd_set                * fd_out;
    fd_set                * fd_ex;
};

#endif /* TCPSERVER_H */

