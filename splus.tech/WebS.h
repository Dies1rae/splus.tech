#pragma once
#include "TcpListen.h"

class WebS : public TcpListen {
  public:
    WebS(const char* IP, uint16_t P) : TcpListen(IP, P) {}

  protected:
    virtual void onClientConnected(client& c) override;
    virtual void onClientDisconnected(client& c) override;
    virtual void onMessageReceived(client& c, std::string_view msg) override;
};
