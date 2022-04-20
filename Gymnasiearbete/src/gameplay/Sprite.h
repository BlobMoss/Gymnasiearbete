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

    bool m_Active;

    Model* m_Model;
    bool m_Highlighted;
    glm::vec4 m_Color;

    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    std::string m_ObjPath;
    std::string m_TexturePath;
    std::string m_ShaderPath;

public:
    Sprite();
    Sprite(Model* model);
    virtual ~Sprite();

    virtual void Update(float deltaTime);

    virtual void Draw();

    virtual void Remove();
    bool WillBeRemoved();

    virtual SpriteTypes GetType() { return SpriteTypes::Sprite; }

    virtual void SetDescription(std::vector<uint8_t>& desc);
    virtual void ForcedSetDescription(std::vector<uint8_t>& desc);
    virtual std::vector<uint8_t> GetDescription() const;
};