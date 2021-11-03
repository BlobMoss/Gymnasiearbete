#pragma once

#include "Renderer.h"

#include "Model.h"

class Engine
{
private:
    Model m_Gem;
    Model m_Teapot;
public:
    Engine();
    ~Engine();

    void Update(float deltaTime);
    void Draw(Renderer renderer);
};