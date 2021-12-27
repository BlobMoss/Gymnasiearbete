#pragma once

#include "Common.h"

#include "gameplay/SpriteManager.h"

class Client : public net::client_interface<MsgTypes>
{
private:

public:
    Sprite* m_Player;

    Client();

    bool waitingToConnect;

    uint16_t m_ClientID;

    void ServerUpdate();

    void OnConnect();

    void PingServer();
};