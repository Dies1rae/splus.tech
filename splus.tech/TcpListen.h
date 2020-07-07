#pragma once
#include "TcpListen.h"
#include <bits/stdint-uintn.h> // for uint16_t
#include <string>              // for string, basic_string
#include <string_view>         // for string_view
#include <sys/select.h>        // for fd_set
#include <vector>              // for vector
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

  protected:
    struct client {
        int socket_fd = -1;
        std::string peer_ip_address;
    };
    virtual void onClientConnected(client&);
    virtual void onClientDisconnected(client&);
    virtual void onMessageReceived(client& c, std::string_view msg);
    void sendToClient(client& c, std::string_view msg);
    void broadcastToClients(client& c, std::string_view msg);

  private:
    const char* svr_addr;
    uint16_t svr_port;
    int svr_socket;
    std::vector<client> clients;
    fd_set* fd_in;
    fd_set* fd_out;
    fd_set* fd_ex;
};
