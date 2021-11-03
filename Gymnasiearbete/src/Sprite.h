#pragma once

#include "Renderer.h"

class Sprite
{
private:

public:
    Sprite();
    virtual ~Sprite();

    virtual void Update(float deltaTime);
    virtual void Draw(Renderer renderer);
};