#include "BlockGroup.h"

BlockGroup::BlockGroup()
{
    SetBlock(glm::ivec3(16, 0, 16), SOLID);

	m_Mesh = GenerateMesh();

	m_Model = new Model(m_Mesh, "res/textures/teapot_texture.png", "res/shaders/lighting.shader");
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

    for (int z = -16; z < 16; z++)
    {
        for (int y = -1; y < 1; y++)
        {
            for (int x = -16; x < 16; x++)
            {
                if (m_Blocks[x + 16][y + 1][z + 16] == SOLID)
                {
                    for (unsigned int i = 0; i < 12; i++)
                    {
                        unsigned int positionIndex[3] = { cubeIndices[i][0], cubeIndices[i][3], cubeIndices[i][6] };
                        unsigned int uvIndex[3] = { cubeIndices[i][1], cubeIndices[i][4], cubeIndices[i][7] };
                        unsigned int normalIndex[3] = { cubeIndices[i][2], cubeIndices[i][5], cubeIndices[i][8] };

                        for (unsigned int i = 0; i < 3; i++)
                        {
                            vertices.push_back(cubePositions[positionIndex[i] - 1].x + x);
                            vertices.push_back(cubePositions[positionIndex[i] - 1].y + y);
                            vertices.push_back(cubePositions[positionIndex[i] - 1].z + z);

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
    m_Blocks[position.x][position.y][position.z] = type;

    m_UpdateNeeded = true;
}
char BlockGroup::GetBlock(glm::ivec3 position)
{
    return m_Blocks[position.x][position.y][position.z];
}