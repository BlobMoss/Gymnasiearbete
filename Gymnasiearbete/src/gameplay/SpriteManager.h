#pragma once

#include "../MessageTypes.h"
#include "../Renderer.h"

#include "Sprite.h"
#include "Body.h"
#include "Player.h"

class Client;

class SpriteManager
{
private:
    static std::unordered_map<int64_t, Sprite*> m_Sprites;

public:
    SpriteManager();
    ~SpriteManager();

    static Client* client;

    static int64_t m_LocalIDCounter;

    static void AddSprite(Sprite* sprite);
    static void AddSprite(int64_t id, Sprite* sprite);
    static void AssignID(int64_t oldID, int64_t newID);

    static void UpdateLocally(float deltaTime);
    static void UpdateServer();
    static void SyncSprite(int64_t id, SpriteDescription desc);
    static void Draw();
};