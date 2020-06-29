#include "TcpListen.h"
#include <iostream>
#include <string>
#include <sstream>

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
				SOCKET client;// = accept(m_socket, (struct sockaddr*)&client_addr, &addrlen);
				if (client = accept(m_socket, (struct sockaddr*)&client_addr, &addrlen)) {
					char ip[16];
					inet_ntop(AF_INET, &(client_addr.sin_addr), ip, 16);
					std::cout << ip << std::endl;
				}
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