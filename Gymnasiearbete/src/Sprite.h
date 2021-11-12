#pragma once

#include "Renderer.h"

#include "Model.h"

class Sprite
{
private:
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;

    Model* m_Model;

public:
    Sprite();
    Sprite(Model* model);
    virtual ~Sprite();

    virtual void Update(float deltaTime);

    virtual void Draw(Renderer renderer);

    void SetPosition(glm::vec3 p);
    glm::vec3 GetPosition();
    void SetRotation(glm::vec3 r);
    glm::vec3 GetRotation();
};