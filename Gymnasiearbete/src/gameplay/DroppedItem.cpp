#include "DroppedItem.h"

DroppedItem::DroppedItem()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int texWidth = m_Model != nullptr ? m_Model->m_Texture.GetWidth() : 16;

    unsigned int index = 0;

    for (unsigned int i = 0; i < 12; i++)
    {
        // Don't put faces between blocks
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

    return { vertices, indices };
}
DroppedItem::~DroppedItem()
{

}

void DroppedItem::Update(float deltaTime)
{

}