#pragma once
#include "../Common.h"

#include "../graphics/Image.h"

#include "../Camera.h"

class UISprite
{
protected:
    bool m_WillBeRemoved;

public:
    bool m_Active;

    glm::ivec2 m_Position;

    float m_SortingOrder;

    Image* m_Image;

public:

    UISprite();
    UISprite(Image* image);
    virtual ~UISprite();

    virtual void Update(float deltaTime);

    virtual void Draw();

    void Remove();
    bool WillBeRemoved();
};

