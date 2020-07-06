/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WebSrv.h
 * Author: dies
 *
 * Created on July 2, 2020, 4:27 PM
 */
#pragma once
#include "TcpServer.h"
#ifndef WEBSRV_H
#define WEBSRV_H


class WebSrv : public TcpServer{
public:
    WebSrv(const char * IP, int P) :TcpServer(IP, P) { }
protected:
    virtual void onClientConnected(int clientSocket);
    virtual void onClientDisconnected(int clientSocket);
    virtual void onMessageReceived(int clientSocket, const char* msg, int length) override;
};

#endif /* WEBSRV_H */

