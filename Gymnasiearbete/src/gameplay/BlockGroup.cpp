#include "BlockGroup.h"

BlockGroup::BlockGroup()
{
    SetBlock(glm::ivec3(16, 0, 16), PLANKS);

	m_Mesh = GenerateMesh();

	m_Model = new Model(m_Mesh, "res/textures/blocks_texture.png", "res/shaders/lighting.shader");
}
BlockGroup::~BlockGroup()
{
    
}

void BlockGroup::Update(float deltaTime)
{
    if (m_UpdateNeeded)
    {
        m_Mesh = GenerateMesh();

        m_Model->UpdateData(m_Mesh);

        m_UpdateNeeded = false;
    }

    Sprite::Update(deltaTime);
}

Mesh BlockGroup::GenerateMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int index = 0;

    for (int z = 0; z < 32; z++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int x = 0; x < 32; x++)
            {
                if (GetBlock(glm::ivec3(x, y, z)) != EMPTY)
                {
                    for (unsigned int i = 0; i < 10; i++)
                    {
                        if (GetBlock(glm::ivec3(x, y + 1, z)) != EMPTY && cubeIndices[i][2] == 1) continue;
                        if (GetBlock(glm::ivec3(x + 1, y, z)) != EMPTY && cubeIndices[i][2] == 2) continue;
                        if (GetBlock(glm::ivec3(x, y, z + 1)) != EMPTY && cubeIndices[i][2] == 3) continue;
                        if (GetBlock(glm::ivec3(x - 1, y, z)) != EMPTY && cubeIndices[i][2] == 4) continue;
                        if (GetBlock(glm::ivec3(x, y, z - 1)) != EMPTY && cubeIndices[i][2] == 5) continue;

                        unsigned int positionIndex[3] = { cubeIndices[i][0], cubeIndices[i][3], cubeIndices[i][6] };
                        unsigned int uvIndex[3] = { cubeIndices[i][1], cubeIndices[i][4], cubeIndices[i][7] };
                        unsigned int normalIndex[3] = { cubeIndices[i][2], cubeIndices[i][5], cubeIndices[i][8] };

                        for (unsigned int i = 0; i < 3; i++)
                        {
                            vertices.push_back(cubePositions[positionIndex[i] - 1].x + x - 16);
                            vertices.push_back(cubePositions[positionIndex[i] - 1].y + y - 1);
                            vertices.push_back(cubePositions[positionIndex[i] - 1].z + z - 16);

                            vertices.push_back(cubeUvs[uvIndex[i] - 1].x);
                            vertices.push_back(cubeUvs[uvIndex[i] - 1].y);

                            vertices.push_back(cubeNormals[normalIndex[i] - 1].x);
                            vertices.push_back(cubeNormals[normalIndex[i] - 1].y);
                            vertices.push_back(cubeNormals[normalIndex[i] - 1].z);

                            indices.push_back(index);
                            index++;
                        }
                    }
                }
            }
        }
    }

    return { vertices, indices };
}

void BlockGroup::SetBlock(glm::ivec3 position, unsigned char type)
{
    // Make sure block is within array
    if (position.x < 0 || position.y < 0 || position.z < 0) return;
    if (position.x >= 32 || position.y >= 2 || position.z >= 32) return;

    m_Blocks[position.x][position.y][position.z] = type;

    m_UpdateNeeded = true;
}
char BlockGroup::GetBlock(glm::ivec3 position)
{
    // Make sure block is within array
    if (position.x < 0 || position.y < 0 || position.z < 0) return EMPTY;
    if (position.x >= 32 || position.y >= 2 || position.z >= 32) return EMPTY;

    return m_Blocks[position.x][position.y][position.z];
}

void BlockGroup::SetDescription(std::vector<uint8_t>& desc)
{
    desc >> m_WillBeRemoved >> m_Blocks >> m_Scale >> m_Rotation >> m_Position;
}
std::vector<uint8_t> BlockGroup::GetDescription() const
{
    std::vector<uint8_t> desc;
    desc << m_Position << m_Rotation << m_Scale << m_Blocks << m_WillBeRemoved;
    return desc;
}