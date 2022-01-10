#include "BlockGroup.h"

BlockGroup::BlockGroup()
{
    SetBlock(glm::ivec3(0, 0, 0), PLANKS);

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

    m_PotentialPosition = glm::vec2(m_Position.x, m_Position.z) + m_Velocity * deltaTime;
    m_PotentialRotation = m_Rotation.y + m_AngularVelocity * deltaTime;

    Sprite::Update(deltaTime);
}

void BlockGroup::OnCollision(Body* body, BlockCollisions side)
{
    if (side == BlockCollisions::Floor)
    {
        if (std::find(m_Bodies.begin(), m_Bodies.end(), body) == m_Bodies.end())
        {
            m_Bodies.push_back(body);
        }
    }
}
void BlockGroup::OnCollision(BlockGroup* blockGroup)
{

}

void BlockGroup::Move()
{
    m_Position = glm::vec3(m_PotentialPosition.x, 0.0f, m_PotentialPosition.y);
    m_Rotation.y = m_PotentialRotation;
}

void BlockGroup::Draw()
{
    Sprite::Draw();
}

Mesh BlockGroup::GenerateMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int texWidth = m_Model != nullptr ? m_Model->m_Texture.GetWidth() : 16;

    unsigned int index = 0;

    for (int z = -32; z < 32; z++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int x = -32; x < 32; x++)
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

                        for (unsigned int ii = 0; ii < 3; ii++)
                        {
                            vertices.push_back(cubePositions[positionIndex[ii] - 1].x + x);
                            vertices.push_back(cubePositions[positionIndex[ii] - 1].y + y - 0.5f);
                            vertices.push_back(cubePositions[positionIndex[ii] - 1].z + z);

                            vertices.push_back((cubeUvs[uvIndex[ii] - 1].x + GetBlock(glm::ivec3(x, y, z)) - 1) * (16.0f / texWidth));
                            unsigned char row = 0;
                            if (normalIndex[ii] == 1)
                                row = 2;
                            else if (GetBlock(glm::ivec3(x, y + 1, z)) == EMPTY)
                                row = 1;
                            vertices.push_back((cubeUvs[uvIndex[ii] - 1].y + row) * (16.0f / 48.0f));

                            vertices.push_back(cubeNormals[normalIndex[ii] - 1].x);
                            vertices.push_back(cubeNormals[normalIndex[ii] - 1].y);
                            vertices.push_back(cubeNormals[normalIndex[ii] - 1].z);

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

void BlockGroup::SetBlock(glm::ivec3 pos, unsigned char type)
{
    // Make sure block is within array
    if (pos.x < -32 || pos.y < 0 || pos.z < -32) return;
    if (pos.x >= 32 || pos.y >= 2 || pos.z >= 32) return;

    m_Blocks[pos.x + 32][pos.y][pos.z + 32] = type;

    m_UpdateNeeded = true;
}
char BlockGroup::GetBlock(glm::ivec3 pos)
{
    // Make sure block is within array
    if (pos.x < -32 || pos.y < 0 || pos.z < -32) return EMPTY;
    if (pos.x >= 32 || pos.y >= 2 || pos.z >= 32) return EMPTY;

    return m_Blocks[pos.x + 32][pos.y][pos.z + 32];
}

void BlockGroup::SetDescription(std::vector<uint8_t>& desc)
{
    unsigned char lastBlocks[64][2][64];
    std::memcpy(lastBlocks, m_Blocks, 64 * 2 * 64 * sizeof(unsigned char));
    desc >> m_WillBeRemoved >> m_Blocks >> m_Scale >> m_Rotation >> m_Position;
    if (std::memcmp(lastBlocks, m_Blocks, 64 * 2 * 64 * sizeof(unsigned char)) != 0.0f) m_UpdateNeeded = true;
}
std::vector<uint8_t> BlockGroup::GetDescription() const
{
    std::vector<uint8_t> desc;
    desc << m_Position << m_Rotation << m_Scale << m_Blocks << m_WillBeRemoved;
    return desc;
}