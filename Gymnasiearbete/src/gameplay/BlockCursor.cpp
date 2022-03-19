#include "BlockCursor.h"

#include "SpriteManager.h"
#include "Raycast.h"

BlockCursor::BlockCursor()
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
    if (glm::length(glm::vec3(hit.lastEmpty - hit.firstBlock)) == 1.0f)
    {
        m_Highlighted = hit;
    }
    if (m_Highlighted.blockGroup == nullptr) return;

    if (Input::MouseButtonHeld(MOUSE_BUTTON_LEFT) && m_Selected.blockGroup == nullptr)
    {
        m_Selected = m_Highlighted;
    }
    if (m_Selected.blockGroup != nullptr) m_Highlighted = m_Selected;
    


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
                m_Selected.blockGroup->SetBlock(m_Selected.firstBlock, EMPTY);
                SpriteManager::ForceUpdate(m_Selected.blockGroup->m_Id);
            }
            m_BreakTime += deltaTime;
        }
        if (Input::MouseButtonDown(MOUSE_BUTTON_2))
        {
            m_Selected.blockGroup->SetBlock(m_Selected.lastEmpty, PLANKS);
            SpriteManager::ForceUpdate(m_Selected.blockGroup->m_Id);
        }
    }
}
void BlockCursor::Draw()
{
    if (m_Visable)
    {
        Sprite::Draw();
    }
}