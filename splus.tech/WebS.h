#pragma once
#ifdef _WIN32
#include "TcpListen.h"


class WebS : public TcpListen {
public:
	WebS(const char* ipAddress, int port) :TcpListen(ipAddress, port) { }
protected:
	virtual void onClientConnected(int clientSocket);
	virtual void onClientDisconnected(int clientSocket);
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);
};
#else
#include "TcpServer.h"

class WebSrv : public TcpServer {
public:
	WebSrv(const char* IP, int P) :TcpServer(IP, P) { }
protected:
	virtual void onClientConnected(int clientSocket);
	virtual void onClientDisconnected(int clientSocket);
	virtual void onMessageReceived(int clientSocket, const char* msg, int length) override;
};

#endif
