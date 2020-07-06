#pragma once
#ifdef _WIN32
#include "TcpListen.h"


class WebS : public TcpListen {
public:
	WebS(const char* ipAddress, uint16_t port) :TcpListen(ipAddress, port) { }
protected:
	virtual void onClientConnected(int clientSocket) override;
	virtual void onClientDisconnected(int clientSocket) override;
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);
};
#else
#include "TcpListen.h"

class WebS : public TcpListen {
public:
	WebS(const char* IP, uint16_t P) :TcpListen(IP, P) { }
protected:
	virtual void onClientConnected(int clientSocket) override;
	virtual void onClientDisconnected(int clientSocket) override;
	virtual void onMessageReceived(int clientSocket, const char* msg, size_t length) override;
};

#endif
