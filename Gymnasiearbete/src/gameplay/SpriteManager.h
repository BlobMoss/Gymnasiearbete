#pragma once

#include "../MessageTypes.h"

#include "Sprite.h"
#include "Body.h"
#include "Player.h"
#include "BlockGroup.h"
#include "DroppedItem.h"

#include "boat_parts/Mast.h"
#include "boat_parts/Helm.h"
#include "boat_parts/Cannon.h"
#include "boat_parts/CannonBall.h"

class Client;

class SpriteManager
{
private:
    static std::unordered_map<int64_t, Sprite*> m_Sprites;
    static std::unordered_map<int64_t, std::vector<uint8_t>> m_LastDescriptions;

    static std::unordered_map<int64_t, Sprite*> m_TempSprites;

    static void AddSpriteInternal(int64_t id, Sprite* sprite);

public:
    static Client* m_Client;

    // Used for assigning temporary ids before shared one arrives
    static int64_t m_LocalIDCounter;

    static Player* m_Player;
    static std::vector<Body*> m_Bodies;
    static std::vector<BlockGroup*> m_BlockGroups;
    static std::vector<BoatPart*> m_BoatParts;

    // Add sprite only for this client
    static void AddSpriteLocally(Sprite* sprite);
    // Add sprite here and on every other client
    static void AddSprite(Sprite* sprite);
    // Add sprite created and shared by another client
    static void AddSprite(int64_t id, SpriteTypes type, std::vector<uint8_t> desc);

    // Replace temporary id with shared id
    static void AssignID(int64_t oldID, int64_t newID);
    // Give this client ownership of abandoned sprite
    static void MakeOwner(int64_t id);
    // Add sprite at a known location
    static void AddSpriteWithID(int64_t id, Sprite* sprite);
    // Remove sprite removed on another client
    static void RemoveSpriteByID(int64_t id);

    // Update every sprite on this client
    static void UpdateLocally(float deltaTime);
    // Update sprites owned by this client for other clients
    static void UpdateServer();
    // Update a sprite, owned by this client or not for other clients
    static void ForceUpdate(uint64_t id);
    // Save state of every sprite to know which have changed
    static void SaveDescriptions();
    // Synchronise sprite with update from another client
    static void SyncSprite(int64_t id, std::vector<uint8_t> desc);
    // Forcefully synchronise sprite with update from another client
    static void ForceSyncSprite(int64_t id, std::vector<uint8_t> desc);

    // Draw sprites as shadows?
    static bool drawingShadows;
    // Draw every sprite
    static void Draw();
};