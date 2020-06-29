#pragma once
#include <WS2tcpip.h>
#include <fstream>
#pragma comment (lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE (49152)
class TcpListen {
public:
	TcpListen(const char* ipAddress, int port) :m_ipAddress(ipAddress), m_port(port), m_socket(NULL), m_master() { }
	int init();
	int run();
protected:
	virtual void onClientConnected(int clientSocket);
	virtual void onClientDisconnected(int clientSocket);
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);
	void sendToClient(int clientSocket, const char* msg, int length);
	void broadcastToClients(int sendingClient, const char* msg, int length);
private:
	const char* m_ipAddress;
	int				m_port;
	int				m_socket;
	fd_set			m_master;
};
