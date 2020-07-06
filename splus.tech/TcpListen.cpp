#ifdef _WIN32
#include "TcpListen.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <set>
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
#endif

void logfile(char*);
#ifdef _WIN32
int TcpListen::init() {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0) {
		return wsOk;
	}
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET) {
		return WSAGetLastError();
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress, &hint.sin_addr);

	if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
		return WSAGetLastError();
	}
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR) {
		return WSAGetLastError();
	}
	FD_ZERO(&m_master);
	FD_SET(m_socket, &m_master);
	return 0;
}

int TcpListen::run() {
	bool running = true;
	while (running) {
		fd_set copy = m_master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == m_socket) {
				SOCKADDR_IN client_addr;
				int addrlen = sizeof(client_addr);
				//get addr client
				SOCKET client;
				if (client = accept(m_socket, (struct sockaddr*)&client_addr, &addrlen)) {
					char tmpIP[16];
					inet_ntop(AF_INET, &(client_addr.sin_addr), tmpIP, 16);
					this->cl_ip_ad.assign(tmpIP, 16);
					logfile(tmpIP);
				}
				//-----
				FD_SET(client, &m_master);
				onClientConnected(client);
			}
			else {
				char buf[MAX_BUFFER_SIZE];
				ZeroMemory(buf, MAX_BUFFER_SIZE);
				int bytesIn = recv(sock, buf, MAX_BUFFER_SIZE, 0);
				if (bytesIn <= 0) {
					onClientDisconnected(sock);
					closesocket(sock);
					FD_CLR(sock, &m_master);
				}
				else {
					onMessageReceived(sock, buf, bytesIn);
				}
			}
		}
	}
	FD_CLR(m_socket, &m_master);
	closesocket(m_socket);
	while (m_master.fd_count > 0) {
		SOCKET sock = m_master.fd_array[0];
		FD_CLR(sock, &m_master);
		closesocket(sock);
	}
	WSACleanup();
	return 0;
}


void TcpListen::sendToClient(int clientSocket, const char* msg, int length) {
	send(clientSocket, msg, length, 0);
}

void TcpListen::broadcastToClients(int sendingClient, const char* msg, int length) {
	for (int i = 0; i < m_master.fd_count; i++) {
		SOCKET outSock = m_master.fd_array[i];
		if (outSock != m_socket && outSock != sendingClient) {
			sendToClient(outSock, msg, length);
			
		}
	}
}
void TcpListen::onClientConnected(int clientSocket) {

}
void TcpListen::onClientDisconnected(int clientSocket) {

}
void TcpListen::onMessageReceived(int clientSocket, const char* msg, int length) {

}

std::string TcpListen::get_cl_ip_addrs() {
	return this->cl_ip_ad;
}
#else
int TcpListen::init() {
	svr_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (svr_socket == -1) {
		cout << "Can't create a socket! Quitting" << endl;
		return -1;
	}
	int opt = 1;
	setsockopt(svr_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(svr_port);
	inet_pton(AF_INET, svr_addr, &hint.sin_addr);

	if (bind(svr_socket, (sockaddr*)&hint, sizeof(hint)))
        throw std::system_error(errno, std::generic_category());

	listen(svr_socket, SOMAXCONN);
	cout << "server listen on: " << svr_port << endl;
	return 0;
}

int TcpListen::run() {
	bool running = true;
	while (running) {
		FD_ZERO(fd_in);
		FD_ZERO(fd_out);
		FD_ZERO(fd_ex);
		FD_SET(svr_socket, fd_in);
		for (int i = 0;i < cl_so_main.size();i++) {
			FD_SET(cl_so_main[i], fd_in);
		}
		int socketCount = select(FD_SETSIZE, fd_in, NULL, NULL, NULL);
		if (FD_ISSET(svr_socket, fd_in)) {
			sockaddr_in client_addr;
			int addrlen = sizeof(client_addr);
			cl_so_main.push_back(accept(svr_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen));
			inet_ntop(AF_INET, &(client_addr.sin_addr), cl_ip_addr, 16);
			logfile(cl_ip_addr);
			onClientConnected(cl_so_main[cl_so_main.size()]);
		}
		for (int i = 0;i < cl_so_main.size();i++) {
			if (FD_ISSET(cl_so_main[i], fd_in)) {
				char buf[MAX_BUFFER_SIZE];
				memset(buf, 0, MAX_BUFFER_SIZE);
				int bytesIn = recv(cl_so_main[i], buf, MAX_BUFFER_SIZE, 0);
				if (bytesIn <= 0) {
					onClientDisconnected(cl_so_main[i]);
					close(cl_so_main[i]);
					FD_CLR(cl_so_main[i], fd_in);
					FD_CLR(cl_so_main[i], fd_out);
					FD_CLR(cl_so_main[i], fd_ex);
					cl_so_main.erase(cl_so_main.begin() + i);
				}
				else {
					onMessageReceived(cl_so_main[i], buf, bytesIn);
				}
			}
		}
	}
	return 0;
}

void TcpListen::sendToClient(int clientSocket, const char* msg, int length) {
	send(clientSocket, msg, length, 0);
}

void TcpListen::broadcastToClients(int sendingClient, const char* msg, int length) {
	for (int i = 0; i < cl_so_main.size(); i++) {
		sendToClient(cl_so_main[i], msg, length);
	}
}

void TcpListen::onClientConnected(int clientSocket) {

}

void TcpListen::onClientDisconnected(int clientSocket) {

}
void TcpListen::onMessageReceived(int clientSocket, const char* msg, int length) {

}
char* TcpListen::get_cl_ip_addrs() {
	return this->cl_ip_addr;
}



#endif

void logfile(char* ip) {
	std::string filepath = ".\\ipaddr.txt";
	std::ifstream check_file(filepath, std::ios::binary | std::ios::ate);
	if (check_file.tellg() >= 4000) {
		std::ofstream ip_file;
		ip_file.open(filepath, std::ofstream::out | std::ofstream::trunc);
		ip_file.close();
	}
	std::ofstream ip_file;
	ip_file.open(filepath, std::ios_base::app);
	ip_file << ip << '\n';
	ip_file.close();
}
