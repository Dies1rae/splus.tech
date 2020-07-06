/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/* 
 * File:   TcpServer.cpp
 * Author: dies irae
 * 
 * Created on July 2, 2020, 4:21 PM
 */
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
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <limits.h>
#include <iterator>
#include <vector>
#include <algorithm> 
using namespace std;

void logfile(char*);

int TcpServer::init() {
    svr_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_socket == -1)   {
        cout << "Can't create a socket! Quitting" << endl;
        return -1;
    }
    int opt = 1;
    setsockopt(svr_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(svr_port);
    inet_pton(AF_INET, svr_addr, &hint.sin_addr);
    
    bind(svr_socket, (sockaddr*)&hint, sizeof(hint));
    
    listen(svr_socket, SOMAXCONN);
    cout << "server listen on: " << svr_socket << endl;
    return 0;
}

int TcpServer::run() {
    bool running = true;
    while (running) {
        FD_ZERO(fd_in);
        FD_ZERO(fd_out);
        FD_ZERO(fd_ex);
        FD_SET(svr_socket, fd_in);  
        for (int i=0;i<cl_so_main.size();i++) {
            FD_SET(cl_so_main[i], fd_in);
        }
        int socketCount = select(FD_SETSIZE, fd_in, NULL, NULL,NULL);
            if (FD_ISSET(svr_socket, fd_in)){
                sockaddr_in client_addr;
                int addrlen = sizeof(client_addr);
                cl_so_main.push_back(accept(svr_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen));
                inet_ntop(AF_INET, &(client_addr.sin_addr), cl_ip_addr, 16);
                logfile(cl_ip_addr);
                onClientConnected(cl_so_main[cl_so_main.size()]);
            }
            for (int i=0;i<cl_so_main.size();i++) {
                if (FD_ISSET(cl_so_main[i], fd_in)) {
                    char buf[MAX_BUFFER_SIZE];
                    memset(buf,0, MAX_BUFFER_SIZE);
                    int bytesIn = recv(cl_so_main[i], buf, MAX_BUFFER_SIZE, 0);
                    if (bytesIn <= 0) {
                        onClientDisconnected(cl_so_main[i]);
                        close(cl_so_main[i]); 
                        FD_CLR(cl_so_main[i], fd_in);
                        FD_CLR(cl_so_main[i],fd_out);
                        FD_CLR(cl_so_main[i],fd_ex);                            
                        cl_so_main.erase(cl_so_main.begin()+i); 
                    }
                    else{
                        onMessageReceived(cl_so_main[i], buf, bytesIn);
                    }
                }
            }
        }
    return 0;
}

void TcpServer::sendToClient(int clientSocket, const char* msg, int length) {
	send(clientSocket, msg, length, 0);
}

void TcpServer::broadcastToClients(int sendingClient, const char* msg, int length) {
	for (int i = 0; i < cl_so_main.size(); i++) {
            sendToClient(cl_so_main[i], msg, length);	
	}
}

void TcpServer::onClientConnected(int clientSocket) {

}

void TcpServer::onClientDisconnected(int clientSocket) {

}
void TcpServer::onMessageReceived(int clientSocket, const char* msg, int length) {

}
char * TcpServer::get_cl_ip_addrs(){
    return this->cl_ip_addr;
}

void logfile(char * ip) {
	std::string filepath = "./ipaddr.txt";
	std::ifstream check_file(filepath, std::ios::binary | std::ios::ate);
	if (check_file.tellg() >= 3000 ) {
		std::ofstream ip_file;
		ip_file.open(filepath, std::ofstream::out | std::ofstream::trunc);
                ip_file << "begin" << '\n';
		ip_file.close();
	}
	std::ofstream ip_file;
	ip_file.open(filepath, std::ios::app);
	ip_file << ip << '\n';
	ip_file.close();
}