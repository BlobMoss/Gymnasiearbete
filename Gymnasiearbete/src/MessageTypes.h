#pragma once

#include <net.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class MsgTypes : uint32_t
{
    Server_Ping,

    Client_Accepted,

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

struct BodyDescription : SpriteDescription
{
    glm::vec3 m_Velocity;
};

struct PlayerDescription : BodyDescription
{
    
};