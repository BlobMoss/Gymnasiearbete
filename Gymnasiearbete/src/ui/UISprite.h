#pragma once
#include "../Common.h"

#include "../graphics/Image.h"

#include "../Camera.h"

class UISprite
{
protected:
    bool m_WillBeRemoved;

public:
    glm::uvec2 m_Position;

public:
    Image* m_Image;

    UISprite();
    UISprite(Image* image);
    virtual ~UISprite();

    virtual void Update(float deltaTime);

    virtual void Draw();

    void Remove();
    bool WillBeRemoved();
};

