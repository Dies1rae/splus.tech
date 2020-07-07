#pragma once
#include "TcpListen.h"

class WebS : public TcpListen {
  public:
    WebS(const char* IP, uint16_t P) : TcpListen(IP, P) {}

  protected:
    virtual void onClientConnected(int clientSocket) override;
    virtual void onClientDisconnected(int clientSocket) override;
    virtual void onMessageReceived(int clientSocket, std::string_view msg) override;
};
