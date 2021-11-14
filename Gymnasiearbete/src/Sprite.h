#pragma once

#include "Renderer.h"

#include "Model.h"

#include "Camera.h";

class Sprite
{
protected:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;
    glm::vec3 m_Scale;

    Model* m_Model;

    bool m_WillBeRemoved;

public:
    Sprite();
    Sprite(Model* model);
    virtual ~Sprite();

    virtual void Update(float deltaTime);

    virtual void Draw(Camera& camera);

    void SetPosition(glm::vec3 p);
    glm::vec3 GetPosition() const;
    void SetRotation(glm::vec3 r);
    glm::vec3 GetRotation() const;
    void SetScale(glm::vec3 s);
    glm::vec3 GetScale() const;

    void Remove();
    bool WillBeRemoved();
};