#include "BlockCursor.h"

#include "SpriteManager.h"
#include "Raycast.h"
#include "Collision.h"

#include "Player.h"

#include "../ui/Inventory.h"

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

    m_CracksModel = new Model(GenerateCracksMesh(), "res/textures/cracks.png", "res/shaders/animated.shader");
    m_CracksModel->m_HasTransparency = true;
    m_CracksModel->m_Shader.Bind();
    m_CracksModel->m_Shader.SetUniform1i("u_FrameCount", 3);
}
BlockCursor::~BlockCursor()
{

}

void BlockCursor::SetTransform(RayHit hit)
{
    m_Highlighted.blockGroup = nullptr;

    bool hitAdjacent = glm::length(glm::vec3(hit.lastEmpty - hit.firstBlock)) == 1.0f;
    bool hitFirstBlockExists = hit.blockGroup->GetBlock(hit.firstBlock) != EMPTY;
    bool hitFirstEmptyIsEmpty = hit.blockGroup->GetBlock(hit.lastEmpty) == EMPTY;
    bool placing = !Input::MouseButtonHeld(MOUSE_BUTTON_LEFT) || m_Level == hit.lastEmpty.y;
    if (hitAdjacent && hitFirstBlockExists && hitFirstEmptyIsEmpty && placing)
    {
        m_Highlighted = hit;
        m_Level = hit.lastEmpty.y;
    }
    if (m_Highlighted.blockGroup == nullptr) return;

    if (Input::MouseButtonHeld(MOUSE_BUTTON_LEFT) && m_Selected.blockGroup == nullptr)
    {
        m_Selected = m_Highlighted;
    }
    if (m_Selected.blockGroup != nullptr)
    {
        bool selectedAdjacent = glm::length(glm::vec3(m_Selected.lastEmpty - m_Selected.firstBlock)) == 1.0f;
        bool selectedFirstBlockExists = m_Selected.blockGroup->GetBlock(m_Selected.firstBlock) != EMPTY;
        bool hitFirstEmptyIsEmpty = m_Selected.blockGroup->GetBlock(hit.lastEmpty) == EMPTY;
        if (selectedAdjacent && selectedFirstBlockExists && hitFirstEmptyIsEmpty)
        {
            m_Highlighted = m_Selected;
            m_Level = m_Selected.lastEmpty.y;
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
        if (Input::MouseButtonHeld(MOUSE_BUTTON_LEFT) && (Inventory::m_Instance->m_HeldItem.count == 0 || (Inventory::m_Instance->m_HeldItem.type > 16 && Inventory::m_Instance->m_HeldItem.count > 0)))
        {
            m_MaxBreak = breakTimes[m_Selected.blockGroup->GetBlock(m_Selected.firstBlock)];
            if (m_BreakTime > m_MaxBreak)
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
        bool hasBlock = Inventory::m_Instance->m_HeldItem.count > 0 && Inventory::m_Instance->m_HeldItem.type <= 16;
        bool addToStatic = m_Highlighted.blockGroup->m_Static || (!m_Highlighted.blockGroup->m_Static && Inventory::m_Instance->m_HeldItem.type != SAND && Inventory::m_Instance->m_HeldItem.type != GRASS);
        if (Input::MouseButtonHeld(MOUSE_BUTTON_LEFT) && hasBlock && addToStatic)
        {
            if (m_PlaceTime <= 0.0f && Collision::BlockSpaceEmpty(m_Highlighted.blockGroup, m_Highlighted.lastEmpty) && m_Highlighted.blockGroup->GetBlock(m_Highlighted.lastEmpty) == EMPTY)
            {
                m_Highlighted.blockGroup->SetBlock(m_Highlighted.lastEmpty, Inventory::m_Instance->m_HeldItem.type);
                Inventory::m_Instance->m_HeldItem.count--;
                Inventory::m_Instance->UpdateSlots();
                SpriteManager::ForceUpdate(m_Highlighted.blockGroup->m_Id);
                m_BreakTime = 0.0f;
                m_Highlighted.blockGroup = nullptr;
                m_Selected.blockGroup = nullptr;

                m_PlaceTime = m_PlaceDelay;
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
                        if (Input::KeyDown(KEY_H))
                        {
                            boatPart = new Helm();
                        }
                        if (Input::KeyDown(KEY_J))
                        {
                            boatPart = new Cannon();
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

    m_PlaceTime -= deltaTime;
    if (Input::MouseButtonUp(MOUSE_BUTTON_LEFT))
    {
        m_PlaceTime = 0.0f;
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

    if (m_Selected.blockGroup != nullptr && (Inventory::m_Instance->m_HeldItem.count == 0 || (Inventory::m_Instance->m_HeldItem.type > 16 && Inventory::m_Instance->m_HeldItem.count > 0)))
    {
        float rot = -m_Selected.blockGroup->m_Rotation.y;
        glm::vec3 offset(
            m_Selected.firstBlock.x * glm::cos(rot) - m_Selected.firstBlock.z * glm::sin(rot),
            m_Selected.firstBlock.y - 0.5f,
            m_Selected.firstBlock.x * glm::sin(rot) + m_Selected.firstBlock.z * glm::cos(rot)
        );

        m_CracksModel->m_Shader.Bind();
        m_CracksModel->m_Shader.SetUniform1i("u_Frame", (int)((m_BreakTime / m_MaxBreak) * 3.0f));
        m_CracksModel->Draw(m_Selected.blockGroup->m_Position + offset, m_Rotation, glm::vec3(1.0f), false);
    }
}

Mesh BlockCursor::GenerateCracksMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int index = 0;

    for (unsigned int i = 0; i < 10; i++)
    {
        unsigned int positionIndex[3] = { cubeIndices[i][0], cubeIndices[i][3], cubeIndices[i][6] };
        unsigned int uvIndex[3] = { cubeIndices[i][1], cubeIndices[i][4], cubeIndices[i][7] };
        unsigned int normalIndex[3] = { cubeIndices[i][2], cubeIndices[i][5], cubeIndices[i][8] };

        for (unsigned int ii = 0; ii < 3; ii++)
        {
            vertices.push_back(cubePositions[positionIndex[ii] - 1].x + cubeNormals[normalIndex[ii] - 1].x * 0.01f);
            vertices.push_back(cubePositions[positionIndex[ii] - 1].y + cubeNormals[normalIndex[ii] - 1].y * 0.01f);
            vertices.push_back(cubePositions[positionIndex[ii] - 1].z + cubeNormals[normalIndex[ii] - 1].z * 0.01f);

            vertices.push_back(cubeUvs[uvIndex[ii] - 1].x);
            vertices.push_back(cubeUvs[uvIndex[ii] - 1].y);

            vertices.push_back(cubeNormals[normalIndex[ii] - 1].x);
            vertices.push_back(cubeNormals[normalIndex[ii] - 1].y);
            vertices.push_back(cubeNormals[normalIndex[ii] - 1].z);

            indices.push_back(index);
            index++;
        }
    }

    return { vertices, indices };
}