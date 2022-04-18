#include "DroppedItem.h"

DroppedItem::DroppedItem(unsigned char type, unsigned int count)
    : m_Time(0), m_DecayTime(0), m_Type(type), m_Count(count), m_CanBePickedUp(true)
{
    m_BlockBlockPlacement = false;

    m_Time = randf() * 3.0f;
    
    if (m_Type != -1) m_Model = GenerateModel(m_Type);

    m_ColliderRadius = 0.4f;
}
DroppedItem::~DroppedItem()
{

}

Model* DroppedItem::GenerateModel(unsigned char type)
{
    if (type <= 16)
    {
        Model* model = new Model(GenerateBlockMesh(), "res/textures/blocks.png", "res/shaders/lighting.shader");
        m_Scale = glm::vec3(0.6f);
        return model;
    }
    else
    {
        Model* model = new Model(GenerateQuadMesh(), "res/images/item_icons.png", "res/shaders/detailed.shader");
        m_Scale = glm::vec3(1.0f);
        model->m_HasTransparency = true;
        return model;
    }
}

Mesh DroppedItem::GenerateBlockMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int texWidth = 48; // There are better ways of doing this

    unsigned int index = 0;

    for (unsigned int i = 0; i < 10; i++)
    {
        unsigned int positionIndex[3] = { cubeIndices[i][0], cubeIndices[i][3], cubeIndices[i][6] };
        unsigned int uvIndex[3] = { cubeIndices[i][1], cubeIndices[i][4], cubeIndices[i][7] };
        unsigned int normalIndex[3] = { cubeIndices[i][2], cubeIndices[i][5], cubeIndices[i][8] };

        for (unsigned int ii = 0; ii < 3; ii++)
        {
            vertices.push_back(cubePositions[positionIndex[ii] - 1].x);
            vertices.push_back(cubePositions[positionIndex[ii] - 1].y + 0.5f);
            vertices.push_back(cubePositions[positionIndex[ii] - 1].z);

            vertices.push_back((cubeUvs[uvIndex[ii] - 1].x + m_Type - 1) * (16.0f / texWidth));
            vertices.push_back((cubeUvs[uvIndex[ii] - 1].y + (normalIndex[ii] == 1) + 1) * (16.0f / 48.0f));

            vertices.push_back(cubeNormals[normalIndex[ii] - 1].x);
            vertices.push_back(cubeNormals[normalIndex[ii] - 1].y);
            vertices.push_back(cubeNormals[normalIndex[ii] - 1].z);

            indices.push_back(index);
            index++;
        }
    }

    return { vertices, indices };
}

Mesh DroppedItem::GenerateQuadMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int index = 0;

    for (unsigned int i = 0; i < 9; i++)
    {
        if (i == 1 || i == 3 || i == 6 || i == 8)
        {
            unsigned int positionIndex[3] = { cubeIndices[i][0], cubeIndices[i][3], cubeIndices[i][6] };
            unsigned int uvIndex[3] = { cubeIndices[i][1], cubeIndices[i][4], cubeIndices[i][7] };
            unsigned int normalIndex[3] = { cubeIndices[i][2], cubeIndices[i][5], cubeIndices[i][8] };

            for (unsigned int ii = 0; ii < 3; ii++)
            {
                vertices.push_back(cubePositions[positionIndex[ii] - 1].x * 0.1f);
                vertices.push_back(cubePositions[positionIndex[ii] - 1].y + 0.5f);
                vertices.push_back(cubePositions[positionIndex[ii] - 1].z);

                vertices.push_back((cubeUvs[uvIndex[ii] - 1].x + m_Type - 17) * (1.0f / 16.0f));
                vertices.push_back(cubeUvs[uvIndex[ii] - 1].y * (1.0f / 2.0f));

                vertices.push_back(cubeNormals[normalIndex[ii] - 1].x);
                vertices.push_back(cubeNormals[normalIndex[ii] - 1].y);
                vertices.push_back(cubeNormals[normalIndex[ii] - 1].z);

                indices.push_back(index);
                index++;
            }
        }
    }

    return { vertices, indices };
}

void DroppedItem::Update(float deltaTime)
{
    m_Time += deltaTime;
    m_Rotation.y = m_Time * 0.5f;

    if (m_DecayTime > 1.0f)
    {
        m_CanBePickedUp = true;
    }
    if (m_DecayTime > 60.0f)
    {
        Remove();
    }

    m_DecayTime += deltaTime;

    Body::Update(deltaTime);
}

void DroppedItem::Draw()
{
    if (m_Model == nullptr) return;

    m_Model->Draw(m_Position + glm::vec3(0.0f, sin(m_Time * 1.5f) * 0.2f + 0.2f, 0.0f), m_Rotation, m_Scale, m_Highlighted);
}

void DroppedItem::OnCollision(Body* body)
{
    if (dynamic_cast<DroppedItem*>(body) != nullptr)
    {
        DroppedItem* otherItem = dynamic_cast<DroppedItem*>(body);
        if (m_Count >= otherItem->m_Count && m_Type == otherItem->m_Type && isStackable[m_Type])
        {
            m_Count += otherItem->m_Count;
            m_PotentialPosition = (m_PotentialPosition + otherItem->m_PotentialPosition) / 2.0f;
            otherItem->Remove();
        }
    }
}

void DroppedItem::SetDescription(std::vector<uint8_t>& desc)
{
    unsigned char lastType = m_Type;
    desc >> m_WillBeRemoved >> m_Count >> m_Type >> m_Velocity >> m_Scale >> m_Rotation >> m_Position;
    if (lastType != m_Type) m_Model = GenerateModel(m_Type);
}
std::vector<uint8_t> DroppedItem::GetDescription() const
{
    std::vector<uint8_t> desc;
    desc << m_Position << m_Rotation << m_Scale << m_Velocity << m_Type << m_Count << m_WillBeRemoved;
    return desc;
}