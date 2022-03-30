#include "BlockCursor.h"

#include "SpriteManager.h"
#include "Raycast.h"
#include "Collision.h"

#include "Player.h"

BlockCursor::BlockCursor(Player* player)
    : m_Player(player)
{
    const std::vector<float> vertices = {
       -0.5f, -0.5f, -0.499f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.499f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.499f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f, -0.499f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    Mesh mesh = { vertices, indices };

	m_Model = new Model(mesh, "res/textures/cursor.png", "res/shaders/standard.shader");
    m_Model->m_HasTransparency = true;
}
BlockCursor::~BlockCursor()
{

}

void BlockCursor::SetTransform(RayHit hit)
{
    m_Highlighted.blockGroup = nullptr;
    if (glm::length(glm::vec3(hit.lastEmpty - hit.firstBlock)) == 1.0f && hit.blockGroup->GetBlock(hit.firstBlock) != EMPTY && hit.blockGroup->GetBlock(hit.lastEmpty) == EMPTY)
    {
        m_Highlighted = hit;
    }
    if (m_Highlighted.blockGroup == nullptr) return;

    if (Input::MouseButtonHeld(MOUSE_BUTTON_LEFT) && m_Selected.blockGroup == nullptr)
    {
        m_Selected = m_Highlighted;
    }
    if (m_Selected.blockGroup != nullptr)
    {
        if (glm::length(glm::vec3(m_Selected.lastEmpty - m_Selected.firstBlock)) == 1.0f && m_Selected.blockGroup->GetBlock(m_Selected.firstBlock) != EMPTY && m_Selected.blockGroup->GetBlock(hit.lastEmpty) == EMPTY)
        {
            m_Highlighted = m_Selected;
        }
    }
}

void BlockCursor::Update(float deltaTime)
{
    if (Input::MouseButtonUp(MOUSE_BUTTON_LEFT) || !m_Visable)
    {
        m_Selected.blockGroup = nullptr;
    }
    if (m_Selected.blockGroup != nullptr)
    {
        if (m_Selected.blockGroup->GetBlock(m_Selected.firstBlock) == EMPTY)
        {
            m_Selected.blockGroup = nullptr;
            m_BreakTime = 0.0f;
        }
    }
    if (m_Selected.blockGroup == nullptr)
    {
        m_BreakTime = 0.0f;
    }
    if (m_Visable && m_Selected.blockGroup != nullptr)
    {
        if (Input::MouseButtonHeld(MOUSE_BUTTON_1))
        {
            if (m_BreakTime > breakTimes[m_Selected.blockGroup->GetBlock(m_Selected.firstBlock)])
            {
                m_Selected.blockGroup->BreakBlock(m_Selected.firstBlock);
                SpriteManager::ForceUpdate(m_Selected.blockGroup->m_Id);
                m_BreakTime = 0.0f;
                m_Highlighted.blockGroup = nullptr;
                m_Selected.blockGroup = nullptr;
            }
            m_BreakTime += deltaTime;
        }
    }
    if (m_Visable && m_Highlighted.blockGroup != nullptr)
    {
        if (Input::MouseButtonDown(MOUSE_BUTTON_2))
        {
            if (Collision::BlockSpaceEmpty(m_Highlighted.blockGroup, m_Highlighted.lastEmpty))
            {
                m_Highlighted.blockGroup->SetBlock(m_Highlighted.lastEmpty, PLANKS);
                SpriteManager::ForceUpdate(m_Highlighted.blockGroup->m_Id);
                m_BreakTime = 0.0f;
                m_Highlighted.blockGroup = nullptr;
                m_Selected.blockGroup = nullptr;
            }
        }

        if (Input::KeyDown(KEY_G) || Input::KeyDown(KEY_H) || Input::KeyDown(KEY_J))
        {
            if (Collision::BlockSpaceEmpty(m_Highlighted.blockGroup, m_Highlighted.lastEmpty))
            {
                if (m_Highlighted.lastEmpty.y == 1)
                {
                    if (m_Highlighted.blockGroup->GetBlock(m_Highlighted.lastEmpty + glm::ivec3(0, -1, 0)) != EMPTY)
                    {
                        BoatPart* boatPart = nullptr;
                        if (Input::KeyDown(KEY_G))
                        {
                            boatPart = new Mast();
                        }
                        if (boatPart != nullptr && m_Highlighted.blockGroup != nullptr)
                        {
                            boatPart->m_Position = m_Position + glm::vec3(0.0f, -0.5f, 0.0f);

                            float dX = m_Position.x - m_Player->m_Position.x;
                            float dZ = m_Position.z - m_Player->m_Position.z;
                            
                            boatPart->m_Rotation.y = -std::atan2(dZ, dX) + glm::pi<float>() / 2.0f;

                            float offset = glm::mod(m_Highlighted.blockGroup->m_Rotation.y, glm::pi<float>() / 2.0f);

                            boatPart->m_Rotation.y = round((boatPart->m_Rotation.y - offset) / (glm::pi<float>() / 2.0f)) * (glm::pi<float>() / 2.0f) + offset;

                            SpriteManager::AddSprite(boatPart);
                        }
                    }
                }
            }
        }
    }
}
void BlockCursor::Draw()
{
    if (m_Highlighted.blockGroup != nullptr)
    {
        float rot = -m_Highlighted.blockGroup->m_Rotation.y;
        glm::vec3 offset(
            m_Highlighted.lastEmpty.x * glm::cos(rot) - m_Highlighted.lastEmpty.z * glm::sin(rot),
            m_Highlighted.lastEmpty.y - 0.5f,
            m_Highlighted.lastEmpty.x * glm::sin(rot) + m_Highlighted.lastEmpty.z * glm::cos(rot)
        );

        m_Position = m_Highlighted.blockGroup->m_Position + offset;

        glm::ivec3 dif = m_Highlighted.lastEmpty - m_Highlighted.firstBlock;

        if (dif.x == 1) m_Rotation = glm::vec3(0.0f, glm::pi<float>() / 2.0f - rot, 0.0f);
        if (dif.x == -1) m_Rotation = glm::vec3(0.0f, -glm::pi<float>() / 2.0f - rot, 0.0f);
        if (dif.y == 1) m_Rotation = glm::vec3(-glm::pi<float>() / 2.0f, 0.0f, 0.0f - rot);
        if (dif.z == 1) m_Rotation = glm::vec3(0.0f, -rot, 0.0f);
        if (dif.z == -1) m_Rotation = glm::vec3(0.0f, glm::pi<float>() - rot, 0.0f);
    }

    if (m_Visable)
    {
        Sprite::Draw();
    }
}