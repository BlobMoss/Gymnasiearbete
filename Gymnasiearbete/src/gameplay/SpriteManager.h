#pragma once

#include "../MessageTypes.h"

#include "Sprite.h"
#include "Body.h"
#include "Player.h"
#include "BlockGroup.h"

class Client;

class SpriteManager
{
private:
    static std::unordered_map<int64_t, Sprite*> m_Sprites;
    static std::unordered_map<int64_t, std::vector<uint8_t>> m_LastDescriptions;

public:
    static Client* client;

    static int64_t m_LocalIDCounter;

    static void AddSprite(Sprite* sprite);
    static void AddSprite(int64_t id, SpriteTypes type, std::vector<uint8_t> desc);
    static void AddSpriteWithID(int64_t id, Sprite* sprite);
    static void AssignID(int64_t oldID, int64_t newID);
    static void RemoveSpriteByID(int64_t id);

    static void UpdateLocally(float deltaTime);
    static void UpdateServer();
    static void SaveDescriptions();
    static void SyncSprite(int64_t id, std::vector<uint8_t> desc);

    static void Draw();
};