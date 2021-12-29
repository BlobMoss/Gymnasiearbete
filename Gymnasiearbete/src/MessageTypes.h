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

/*
struct SpriteDesc
{
    int64_t id = 0;
    SpriteTypes type;
    std::vector<uint8_t> body;
};

template<typename DataType>
static SpriteDesc& operator << (SpriteDesc& desc, const DataType& data)
{
    size_t i = desc.body.size();

    desc.body.resize(desc.body.size() + sizeof(DataType));

    std::memcpy(desc.body.data() + i, &data, sizeof(DataType));

    return desc;
}

template<typename DataType>
static SpriteDesc& operator >> (SpriteDesc& desc, DataType& data)
{
    size_t i = desc.body.size() - sizeof(DataType);

    std::memcpy(&data, desc.body.data() + i, sizeof(DataType));

    desc.body.resize(i);

    return desc;
}

static bool operator!=(const SpriteDesc& a, const SpriteDesc& b)
{
    return a.body == b.body;
}


struct SpriteDescription
{
    SpriteTypes type;

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

struct BlockGroupDescription : SpriteDescription
{
    std::vector<std::vector<std::vector<unsigned char>>> blocks;
};
static bool operator==(const BlockGroupDescription& a, const BlockGroupDescription& b)
{
    return
        a.position == b.position &&
        a.rotation == b.rotation &&
        a.scale == b.scale &&
        a.willBeRemoved == b.willBeRemoved &&
        std::equal(std::begin(a.blocks), std::end(a.blocks), std::begin(b.blocks));
}
// */