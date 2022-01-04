#pragma once

#include "UISprite.h"
#include "UIText.h"

class UISpriteManager
{
private:
    static std::vector<UISprite*> m_UISprites;

public:
    static void AddSprite(UISprite* uiSprite);

    static void Update(float deltaTime);

    static void Draw();
};

