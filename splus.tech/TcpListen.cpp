#include "TcpListen.h"
#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <iterator>
#include <climits>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <streambuf>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

static void logfile(std::string_view ip);

int TcpListen::init() {
    svr_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_socket == -1) {
        std::cout << "Can't create a socket! Quitting" << std::endl;
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
    std::cout << "server listen on: " << svr_port << std::endl;
    return 0;
}

int TcpListen::run() {
    bool running = true;
    while (running) {
        FD_ZERO(fd_in);
        FD_ZERO(fd_out);
        FD_ZERO(fd_ex);
        FD_SET(svr_socket, fd_in);
        for (auto& client : clients) {
            FD_SET(client.socket_fd, fd_in);
        }
        /*int socketCount =*/select(FD_SETSIZE, fd_in, nullptr, nullptr, nullptr);
        if (FD_ISSET(svr_socket, fd_in)) {
            sockaddr_in client_addr;
            int addrlen = sizeof(client_addr);
            auto& newclient = clients.emplace_back();
            newclient.socket_fd = accept(
                    svr_socket,
                    (struct sockaddr*)&client_addr,
                    (socklen_t*)&addrlen);


            newclient.peer_ip_address = [&] {
                std::string ip;
                ip.resize(17, '\0');
                inet_ntop(AF_INET, &(client_addr.sin_addr), ip.data(), 16);
                ip.resize(strlen(ip.data()));
                return ip;
            }();

            logfile(newclient.peer_ip_address);
            onClientConnected(newclient);
        }
        for (auto& c : clients) {
            int& fd = c.socket_fd; // by reference!
            if (FD_ISSET(fd, fd_in)) {
                char buf[MAX_BUFFER_SIZE];
                memset(buf, 0, MAX_BUFFER_SIZE);
                ssize_t bytesIn = recv(fd, buf, MAX_BUFFER_SIZE, 0);

                if (bytesIn <= 0) {
                    onClientDisconnected(c);
                    close(fd);
                    FD_CLR(fd, fd_in);
                    FD_CLR(fd, fd_out);
                    FD_CLR(fd, fd_ex);
                    fd = -1;
                } else {
                    if (bytesIn > 0) {
                        onMessageReceived(c, std::string_view(buf, static_cast<size_t>(bytesIn)));
                    }
                }
            }
        }

        clients.erase(
            std::remove_if(
                begin(clients), end(clients),
                [](client const& c) { return c.socket_fd == -1; }),
            end(clients));
    }
    return 0;
}

void TcpListen::sendToClient(client& client, std::string_view msg) {
    send(client.socket_fd, msg.data(), msg.length(), 0);
}

void TcpListen::broadcastToClients(client&, std::string_view msg) {
    for (client& c : clients)
        sendToClient(c, msg);
}

void TcpListen::onClientConnected(client&) {}
void TcpListen::onClientDisconnected(client&) {}
void TcpListen::onMessageReceived(client&, std::string_view) {}

void logfile(std::string_view ip) {
    std::string const filepath = "ipaddr.txt";
    {
        std::ifstream check_file(filepath, std::ios::binary | std::ios::ate);
        if (check_file.tellg() >= 4000) { // TODO FIXME log rotation should probably be someone else's job
            std::ofstream ip_file;
            ip_file.open(filepath, std::ofstream::out | std::ofstream::trunc);
            ip_file.close();
        }
    }
    std::ofstream(filepath, std::ios_base::app) << ip << '\n';
}
