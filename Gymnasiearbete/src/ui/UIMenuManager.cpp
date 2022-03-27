#include "UIMenuManager.h"
#include "UISpriteManager.h"
#include "../Input.h"

UIMenuManager::UIMenuManager()
{
    unsigned int width, height;
    // Main Menu:

    m_StartButton = new UIButton(new Image("res/images/dummy_image.png"));
    width = m_StartButton->m_Image->GetWidth();
    height = m_StartButton->m_Image->GetHeight();
    m_StartButton->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + 80);
    UISpriteManager::AddSprite(m_StartButton);

    m_HostButton = new UIButton(new Image("res/images/dummy_image.png"));
    width = m_HostButton->m_Image->GetWidth();
    height = m_HostButton->m_Image->GetHeight();
    m_HostButton->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + 0);
    UISpriteManager::AddSprite(m_HostButton);

    m_ExitButton = new UIButton(new Image("res/images/dummy_image.png"));
    width = m_ExitButton->m_Image->GetWidth();
    height = m_ExitButton->m_Image->GetHeight();
    m_ExitButton->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) - 80);
    UISpriteManager::AddSprite(m_ExitButton);

    // Join menu:

    m_IPImage = new UISprite(new Image("res/images/dummy_image.png"));
    width = m_IPImage->m_Image->GetWidth();
    height = m_IPImage->m_Image->GetHeight();
    m_IPImage->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + 80);
    UISpriteManager::AddSprite(m_IPImage);

    m_IPTextField = new UITextField();
    m_IPTextField->m_Position = glm::uvec2(referenceWidth / 2, (referenceHeight / 2) + 20);
    UISpriteManager::AddSprite(m_IPTextField);

    m_IPAcceptButton = new UIButton(new Image("res/images/dummy_image.png"));
    width = m_IPAcceptButton->m_Image->GetWidth();
    height = m_IPAcceptButton->m_Image->GetHeight();
    m_IPAcceptButton->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + -40);
    UISpriteManager::AddSprite(m_IPAcceptButton);

    m_IPBackButton = new UIButton(new Image("res/images/dummy_image.png"));
    width = m_IPBackButton->m_Image->GetWidth();
    height = m_IPBackButton->m_Image->GetHeight();
    m_IPBackButton->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + -120);
    UISpriteManager::AddSprite(m_IPBackButton);

    // Connecting menu:

    m_ConnectingImage = new UISprite(new Image("res/images/dummy_image.png"));
    width = m_ConnectingImage->m_Image->GetWidth();
    height = m_ConnectingImage->m_Image->GetHeight();
    m_ConnectingImage->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + 0);
    UISpriteManager::AddSprite(m_ConnectingImage);

    // Connection failed menu

    m_FailedImage = new UISprite(new Image("res/images/dummy_image.png"));
    width = m_FailedImage->m_Image->GetWidth();
    height = m_FailedImage->m_Image->GetHeight();
    m_FailedImage->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + 40);
    UISpriteManager::AddSprite(m_FailedImage);

    m_FailedBackButton = new UIButton(new Image("res/images/dummy_image.png"));
    width = m_FailedBackButton->m_Image->GetWidth();
    height = m_FailedBackButton->m_Image->GetHeight();
    m_FailedBackButton->m_Position = glm::uvec2((referenceWidth - width) / 2, ((referenceHeight - height) / 2) + -40);
    UISpriteManager::AddSprite(m_FailedBackButton);
}
UIMenuManager::~UIMenuManager()
{
    m_StartButton->Remove();
    m_HostButton->Remove();
    m_ExitButton->Remove();

    m_IPImage->Remove();
    m_IPTextField->Remove();
    m_IPAcceptButton->Remove();
    m_IPBackButton->Remove();

    m_ConnectingImage->Remove();

    m_FailedImage->Remove();
    m_FailedBackButton->Remove();
}

std::string UIMenuManager::GetIPInput()
{
    return m_IPTextField->m_TextValue;
}

void UIMenuManager::Update(float deltaTime)
{
    if (m_MenuState == MenuState::Main)
    {
        m_StartButton->m_Active = true;
        m_HostButton->m_Active = true;
        m_ExitButton->m_Active = true;

        m_IPImage->m_Active = false;
        m_IPTextField->m_Active = false;
        m_IPAcceptButton->m_Active = false;
        m_IPBackButton->m_Active = false;

        m_ConnectingImage->m_Active = false;

        m_FailedImage->m_Active = false;
        m_FailedBackButton->m_Active = false;

        if (m_StartButton->Up())
        {
            m_MenuState = MenuState::InputIP;
        }

        if (m_HostButton->Up())
        {
            Renderer::hostGame = true;
        }

        if (m_ExitButton->Up() || Input::KeyDown(KEY_ESCAPE))
        {
            Renderer::exitGame = true;
        }
    }

    if (m_MenuState == MenuState::InputIP)
    {
        m_StartButton->m_Active = false;
        m_HostButton->m_Active = false;
        m_ExitButton->m_Active = false;

        m_IPImage->m_Active = true;
        m_IPTextField->m_Active = true;
        m_IPAcceptButton->m_Active = true;
        m_IPBackButton->m_Active = true;

        m_ConnectingImage->m_Active = false;

        m_FailedImage->m_Active = false;
        m_FailedBackButton->m_Active = false;

        if (m_IPAcceptButton->Up() || Input::KeyDown(KEY_ENTER))
        {
            Renderer::startGame = true;
            m_MenuState = MenuState::Connecting;
        }

        if (m_IPBackButton->Up() || Input::KeyDown(KEY_ESCAPE))
        {
            m_MenuState = MenuState::Main;
        }
    }

    if (m_MenuState == MenuState::Connecting)
    {
        m_StartButton->m_Active = false;
        m_HostButton->m_Active = false;
        m_ExitButton->m_Active = false;

        m_IPImage->m_Active = false;
        m_IPTextField->m_Active = false;
        m_IPAcceptButton->m_Active = false;
        m_IPBackButton->m_Active = false;

        m_ConnectingImage->m_Active = true;

        m_FailedImage->m_Active = false;
        m_FailedBackButton->m_Active = false;
    }

    if (m_MenuState == MenuState::ConnectionFailed)
    {
        m_StartButton->m_Active = false;
        m_HostButton->m_Active = false;
        m_ExitButton->m_Active = false;

        m_IPImage->m_Active = false;
        m_IPTextField->m_Active = false;
        m_IPAcceptButton->m_Active = false;
        m_IPBackButton->m_Active = false;

        m_ConnectingImage->m_Active = false;

        m_FailedImage->m_Active = true;
        m_FailedBackButton->m_Active = true;

        if (m_FailedBackButton->Up() || Input::KeyDown(KEY_ESCAPE))
        {
            m_MenuState = MenuState::InputIP;
        }
    }
}
