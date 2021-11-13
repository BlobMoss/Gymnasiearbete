#pragma once

#include "Sprite.h"

#include "Renderer.h"

class BlockMesh :
    public Sprite
{
private:
    char m_Blocks[32][32];

    Model GenerateModel();

public:
    BlockMesh();
    ~BlockMesh();

    void SetBlock();
    char GetBlock();
};
