#pragma once

#include "../Common.h"

#include "../graphics/Model.h"

#include "../Camera.h"

class Sprite
{
protected:
    bool m_WillBeRemoved;

public:
    bool m_OwnedHere;
    int64_t m_Id;

    Model* m_Model;

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

public:
    Sprite();
    Sprite(Model* model);
    virtual ~Sprite();

    virtual void Update(float deltaTime);

    virtual void Draw();

    void Remove();
    bool WillBeRemoved();

    virtual SpriteTypes GetType() { return SpriteTypes::Sprite; }

    virtual void SetDescription(std::vector<uint8_t>& desc);
    virtual void ForcedSetDescription(std::vector<uint8_t>& desc);
    virtual std::vector<uint8_t> GetDescription() const;
};