#pragma once
#include "../Common.h"

#include "../graphics/Image.h"

#include "../Camera.h"

class UISprite
{
protected:
    glm::uvec2 m_Position;

    Image* m_Image;

    bool m_WillBeRemoved;

public:
    UISprite();
    UISprite(Image* image);
    virtual ~UISprite();

    virtual void Update(float deltaTime);

    virtual void Draw();

    void SetPosition(glm::uvec2 p);
    glm::uvec2 GetPosition() const;

    void Remove();
    bool WillBeRemoved();
};

