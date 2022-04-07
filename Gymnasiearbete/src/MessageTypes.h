#pragma once

#include <net.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class MsgTypes : uint32_t
{
    Server_Ping,

    Client_Accepted,
    Client_RegisterWithServer,
    Client_RegisterID,
    Client_MakeOwner,

    Game_AddSprite,
    Game_AssignID,
    Game_AddSpriteWithID,
    Game_RemoveSprite,
    Game_UpdateSprite,
};

enum class SpriteTypes : uint32_t
{
    Sprite,
    Body,

    Player,
    BlockGroup,
    DroppedItem,
    Mast,
    Helm,
    Cannon,
};

template<typename DataType>
static std::vector<uint8_t>& operator << (std::vector<uint8_t>& desc, const DataType& data)
{
    static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

    size_t i = desc.size();

    desc.resize(desc.size() + sizeof(DataType));

    std::memcpy(desc.data() + i, &data, sizeof(DataType));

    return desc;
}

template<typename DataType>
static std::vector<uint8_t>& operator >> (std::vector<uint8_t>& desc, DataType& data)
{
    static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

    size_t i = desc.size() - sizeof(DataType);

    std::memcpy(&data, desc.data() + i, sizeof(DataType));

    desc.resize(i);

    return desc;
}
