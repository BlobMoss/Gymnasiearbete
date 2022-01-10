#pragma once

#include "Body.h"

#include "BlockData.h"

class BlockGroup : public Sprite
{
protected:
    unsigned char m_Blocks[64][2][64];

    bool m_UpdateNeeded = true;

    std::vector<Body*> m_Bodies;

    Mesh m_Mesh;
    Mesh GenerateMesh();

public:
    BlockGroup();
    ~BlockGroup();

    glm::vec2 m_Velocity;
    float m_AngularVelocity;
    glm::vec2 m_PotentialPosition;
    float m_PotentialRotation;

    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Body* body, BlockCollisions side);
    virtual void OnCollision(BlockGroup* blockGroup);
    virtual void Move();
    virtual void Draw() override;

    void SetBlock(glm::ivec3 position, unsigned char type);
    char GetBlock(glm::ivec3 position);

    virtual SpriteTypes GetType() override { return SpriteTypes::BlockGroup; }

    virtual void SetDescription(std::vector<uint8_t>& desc) override;
    virtual std::vector<uint8_t> GetDescription() const override;
};
