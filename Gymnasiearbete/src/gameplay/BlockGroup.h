#pragma once

#include "Body.h"

#include "BlockData.h"

class BlockGroup : public Sprite
{
protected:
    unsigned char m_Blocks[64][2][64];

    bool m_UpdateNeeded = true;

    std::vector<Body*> m_Bodies;

    void UpdateMass();

    Mesh GenerateMesh();

    bool IsSafe(bool processed[64][2][64], glm::ivec3 coord);
    void FindIsland(bool processed[64][2][64], glm::ivec3 coord);
    void Split();

public:
    BlockGroup();
    ~BlockGroup();

    bool m_Static;
    bool controllable; // Delete later

    float m_Mass, m_InvMass;
    float m_Inertia, m_InvInertia;

    glm::vec2 m_Velocity;
    glm::vec2 m_PotentialPosition;
    float m_AngularVelocity;
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
