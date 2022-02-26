#include "BlockCursor.h"

#include "SpriteManager.h"
#include "Raycast.h"

BlockCursor::BlockCursor()
{
    const std::vector<float> vertices = {
       -0.5f, -0.5f, -0.49f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.49f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.49f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f, -0.49f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
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
        m_BlockGroup = hit.blockGroup;
        m_LastEmpty = hit.lastEmpty;
        m_FirstBlock = hit.firstBlock;
    }

    float rot = -m_BlockGroup->m_Rotation.y;
    glm::vec3 offset(
        m_LastEmpty.x * glm::cos(rot) - m_LastEmpty.z * glm::sin(rot),
        m_LastEmpty.y - 0.5f,
        m_LastEmpty.x * glm::sin(rot) + m_LastEmpty.z * glm::cos(rot)
    );

    m_Position = m_BlockGroup->m_Position + offset;

    glm::ivec3 dif = m_LastEmpty - m_FirstBlock;

    if (dif.x == 1) m_Rotation = glm::vec3(0.0f, glm::pi<float>() / 2.0f - rot, 0.0f);
    if (dif.x == -1) m_Rotation = glm::vec3(0.0f, -glm::pi<float>() / 2.0f - rot, 0.0f);
    if (dif.y == 1) m_Rotation = glm::vec3(-glm::pi<float>() / 2.0f, 0.0f, 0.0f - rot);
    if (dif.z == 1) m_Rotation = glm::vec3(0.0f, -rot, 0.0f);
    if (dif.z == -1) m_Rotation = glm::vec3(0.0f, glm::pi<float>() - rot, 0.0f);
}

void BlockCursor::Update(float deltaTime)
{
    if (m_Visable)
    {
        if (Input::MouseButtonDown(MOUSE_BUTTON_1))
        {
            m_BlockGroup->SetBlock(m_FirstBlock, EMPTY);
            SpriteManager::ForceUpdate(m_BlockGroup->m_Id);
        }
        if (Input::MouseButtonDown(MOUSE_BUTTON_2))
        {
            m_BlockGroup->SetBlock(m_LastEmpty, PLANKS);
            SpriteManager::ForceUpdate(m_BlockGroup->m_Id);
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