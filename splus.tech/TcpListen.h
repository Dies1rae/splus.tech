#pragma once
#ifdef _WIN32
#include <WS2tcpip.h>
#include <fstream>
#include <string>
#pragma comment (lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE (49152)
#else
#include "TcpListen.h"
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
#endif

#ifdef _WIN32
class TcpListen {
public:
	TcpListen(const char* ipAddress, uint16_t port) :m_ipAddress(ipAddress), m_port(port), m_socket(NULL), m_master(), cl_ip_ad() { }
	std::string get_cl_ip_addrs();
	int init();
	int run();
protected:
	virtual void onClientConnected(int clientSocket);
	virtual void onClientDisconnected(int clientSocket);
	virtual void onMessageReceived(int clientSocket, const char* msg, size_t length);
	void sendToClient(int clientSocket, const char* msg, size_t length);
	void broadcastToClients(int sendingClient, const char* msg, size_t length);
private:
	const char*		m_ipAddress;
	uint16_t	    m_port;
	int				m_socket;
	std::string		cl_ip_ad;
	fd_set			m_master;
};
#else
class TcpListen {
public:
    TcpListen(const char* ipAddress, uint16_t port) {
        svr_addr = ipAddress;
        svr_port = port;
        std::fill(std::begin(cl_ip_addr), std::end(cl_ip_addr), 0);
        fd_in = new fd_set;
        fd_out = new fd_set;
        fd_ex = new fd_set;
    }
    int init();
    int run();
    char* get_cl_ip_addrs();
protected:
    virtual void onClientConnected(int clientSocket);
    virtual void onClientDisconnected(int clientSocket);
    virtual void onMessageReceived(int clientSocket, const char* msg, size_t length);
    void sendToClient(int clientSocket, const char* msg, size_t length);
    void broadcastToClients(int sendingClient, const char* msg, size_t length);
private:
    const char* svr_addr;
    uint16_t              svr_port;
    int                   svr_socket;
    vector<int>           cl_so_main;
    char                  cl_ip_addr[16];
    fd_set* fd_in;
    fd_set* fd_out;
    fd_set* fd_ex;
};
#endif
