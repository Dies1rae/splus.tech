#pragma once
#include "Util.h" // the worst name ever

#include "TcpListen.h"
#include <algorithm>
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <set>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#define MAX_BUFFER_SIZE (49152)

class TcpListen {
  public:
    TcpListen(const char* ipAddress, uint16_t port) {
        svr_addr = ipAddress;
        svr_port = port;
        fd_in = new fd_set;
        fd_out = new fd_set;
        fd_ex = new fd_set;
    }
    int init();
    int run();
    std::string get_cl_ip_addrs() const;

  protected:
    virtual void onClientConnected(int clientSocket);
    virtual void onClientDisconnected(int clientSocket);
    virtual void onMessageReceived(int clientSocket, std::string_view msg);
    void sendToClient(int clientSocket, std::string_view msg);
    void broadcastToClients(int sendingClient, std::string_view msg);

  private:
    const char* svr_addr;
    uint16_t svr_port;
    int svr_socket;
    std::vector<int> cl_so_main;
    std::string cl_ip_addr;
    fd_set* fd_in;
    fd_set* fd_out;
    fd_set* fd_ex;
};
