#pragma once

#include "Body.h"

#include "BlockData.h"

struct Fern {
    glm::vec2 position;
    float rotation;
};

struct Tree { 
    glm::ivec2 blockPosition;
    glm::vec2 position;
    float rotation;
};

class BlockGroup : public Sprite
{
protected:
    unsigned char m_Blocks[64][2][64];

    bool m_UpdateNeeded = true;

    std::vector<Body*> m_Bodies;

    void UpdateMass();

    void UpdateRadius();

    Mesh GenerateMesh();
    static Model* m_FernModel;
    std::vector<Fern> m_Ferns;
    static Model* m_TreeModel;
    std::vector<Tree> m_Trees;

    bool IsSafe(bool processed[64][2][64], glm::ivec3 coord);
    void FindIsland(bool processed[64][2][64], glm::ivec3 coord);
    void Split();

public:
    BlockGroup();
    ~BlockGroup();

    bool m_Static;

    float m_Mass, m_InvMass;
    float m_Inertia, m_InvInertia;
    float m_MaxRadius;

    glm::vec2 m_Velocity;
    glm::vec2 m_PotentialPosition;
    float m_AngularVelocity;
    float m_PotentialRotation;

    virtual void Update(float deltaTime) override;
    virtual void OnCollision(Body* body, BlockCollisions side);
    virtual void OnCollision(BlockGroup* blockGroup);
    virtual void Move();
    virtual void Draw() override;

    void AddTree(glm::vec2 pos);

    void Activate();
    void Deactivate();

    void BreakBlock(glm::ivec3 pos);

    void SetBlock(glm::ivec3 pos, unsigned char type);
    char GetBlock(glm::ivec3 pos);

    virtual SpriteTypes GetType() override { return SpriteTypes::BlockGroup; }

    virtual void SetDescription(std::vector<uint8_t>& desc) override;
    virtual std::vector<uint8_t> GetDescription() const override;
};
