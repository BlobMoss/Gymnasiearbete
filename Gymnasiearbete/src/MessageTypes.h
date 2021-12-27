#pragma once

#include <net.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class MsgTypes : uint32_t
{
    Server_Ping,

    Client_Accepted,
    Client_RegisterWithServer,

    Game_AssignID,
    Game_UpdateSprite,
    Game_AddSprite,
    Game_RemoveSprite,
    //Game_UpdateBody,
    //Game_UpdatePlayer,
};

struct SpriteDescription
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    bool willBeRemoved;
};
static bool operator==(const SpriteDescription& a, const SpriteDescription& b)
{
    return 
        a.position == b.position && 
        a.rotation == b.rotation && 
        a.scale == b.scale && 
        a.willBeRemoved == b.willBeRemoved;
}

struct BodyDescription : SpriteDescription
{
    glm::vec3 m_Velocity;
};

struct PlayerDescription : BodyDescription
{
    
};