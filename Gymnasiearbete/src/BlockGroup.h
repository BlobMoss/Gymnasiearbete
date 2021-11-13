#pragma once

#include "Sprite.h"

#include "Renderer.h"

#include "BlockData.h"

class BlockGroup : public Sprite
{
protected:
    unsigned char m_Blocks[32][2][32];

    bool m_UpdateNeeded;

    Mesh m_Mesh;

    Mesh GenerateMesh();

public:
    BlockGroup();
    ~BlockGroup();

    void Update(float deltaTime) override;

    void SetBlock(glm::ivec3 position, unsigned char type);
    char GetBlock(glm::ivec3 position);
};
