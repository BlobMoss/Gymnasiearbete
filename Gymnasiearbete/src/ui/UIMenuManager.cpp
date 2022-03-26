#include "UIMenuManager.h"
#include "UISpriteManager.h"

UIMenuManager::UIMenuManager()
{
    startButton = new UIButton(new Image("res/images/dummy_image.png"));
    unsigned int startWidth = startButton->m_Image->GetWidth(), startHeight = startButton->m_Image->GetHeight();
    startButton->m_Position = glm::uvec2((referenceWidth - startWidth) / 2, ((referenceHeight - startHeight) / 2) + 50);
    UISpriteManager::AddSprite(startButton);

    exitButton = new UIButton(new Image("res/images/dummy_image.png"));
    unsigned int exitWidth = exitButton->m_Image->GetWidth(), exitHeight = exitButton->m_Image->GetHeight();
    exitButton->m_Position = glm::uvec2((referenceWidth - exitWidth) / 2, ((referenceHeight - exitHeight) / 2) - 50);
    UISpriteManager::AddSprite(exitButton);
}
UIMenuManager::~UIMenuManager()
{
    startButton->Remove();
    //hostButton->Remove();
    exitButton->Remove();
}

void UIMenuManager::Update(float deltaTime)
{
    if (startButton->Down())
    {
        Renderer::gameStarted = true;
    }

    if (exitButton->Down())
    {
        Renderer::closeGame = true;
    }
}
