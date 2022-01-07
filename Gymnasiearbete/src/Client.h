#pragma once

#include "Common.h"

#include "gameplay/SpriteManager.h"

class Client : public net::client_interface<MsgTypes>
{
private:

public:
    Client();

    bool waitingToConnect;

    static uint32_t m_ClientID;

    void ServerUpdate();

    void OnRegister();

    void PingServer();
};