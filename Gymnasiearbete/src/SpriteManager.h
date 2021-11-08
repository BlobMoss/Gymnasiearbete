#pragma once

#include "Renderer.h"

#include "Model.h"

class SpriteManager
{
private:
    Model m_Gem;
    Model m_Teapot;
public:
    SpriteManager();
    ~SpriteManager();

    void Update(float deltaTime);
    void Draw(Renderer renderer);
};