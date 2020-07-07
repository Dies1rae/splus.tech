#pragma once
#include "TcpListen.h"         // for TcpListen
#include <bits/stdint-uintn.h> // for uint16_t
#include <string_view>         // for string_view

class WebS : public TcpListen {
  public:
    WebS(const char* IP, uint16_t P) : TcpListen(IP, P) {}

  protected:
    virtual void onClientConnected(client& c) override;
    virtual void onClientDisconnected(client& c) override;
    virtual void onMessageReceived(client& c, std::string_view msg) override;
};
