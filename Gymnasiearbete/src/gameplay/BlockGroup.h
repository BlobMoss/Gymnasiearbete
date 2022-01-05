#pragma once

#include "Sprite.h"

#include "BlockData.h"

class BlockGroup : public Sprite
{
protected:
    unsigned char m_Blocks[64][2][64];

    bool m_UpdateNeeded = true;

    Mesh m_Mesh;

    Mesh GenerateMesh();

public:
    BlockGroup();
    ~BlockGroup();

    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void SetBlock(glm::ivec3 position, unsigned char type);
    char GetBlock(glm::ivec3 position);

    virtual SpriteTypes GetType() override { return SpriteTypes::BlockGroup; }

    virtual void SetDescription(std::vector<uint8_t>& desc) override;
    virtual std::vector<uint8_t> GetDescription() const override;
};
