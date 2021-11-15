#pragma once

#include "Renderer.h"

#include "Sprite.h"

class SpriteManager
{
private:
    std::vector<Sprite*> m_Sprites;

public:
    SpriteManager();
    ~SpriteManager();

    void AddSprite(Sprite* sprite);

    void Update(float deltaTime);
    void Draw();
};