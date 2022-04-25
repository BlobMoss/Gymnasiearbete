#include "Water.h"

#include "../gameplay/World.h"

Water::Water()
{
	m_Mesh = GenerateMesh();

	m_Model = new Model(m_Mesh, "res/textures/water.png", "res/shaders/water.shader");
    m_Model->m_HasTransparency = true;
    m_Model->m_CastsShadow = false;
}
Water::~Water()
{

}

void Water::Draw() 
{
    glm::vec3 pos = Camera::GetFollowTarget()->m_Position;
    glm::ivec2 chunkPos(std::round(pos.x / 17.999f), std::round(pos.z / 17.999f));

    float widthInChunks = (worldWidth / 17.999f) / 2.0f;
    float heightInChunks = (worldHeight / 17.999f) / 2.0f;

    for (int x = chunkPos.x - 4; x < chunkPos.x + 4; x++)
    {
        for (int z = chunkPos.y - 4; z < chunkPos.y + 4; z++)
        {
            glm::vec3 offset(x * 17.999f - 8, 0.0f, z * 17.999f - 8);
            bool edgeChunk = x < -widthInChunks || x > widthInChunks || z < -heightInChunks || z > heightInChunks;
            glm::vec4 color = edgeChunk ? glm::vec4(0.45f, 0.45f, 0.5f, 1.0f) : glm::vec4(1.0f);

            m_Model->m_Shader.Bind();
            m_Model->m_Shader.SetUniform1f("u_Time", (float)glfwGetTime() * 0.6f);
            m_Model->m_Shader.SetUniform4f("u_Offset", offset.x, 0.0f, offset.z, 0.0f);
            m_Model->Draw(offset, m_Rotation, m_Scale, color, m_Highlighted);
        }
    }
}

Mesh Water::GenerateMesh()
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const unsigned int width = 12 + 1;
    const unsigned int length = 12 + 1;

    for (int x = 0; x < width; x++)
    {
        for (int z = 0; z < length; z++)
        {
            vertices.push_back(x * 1.5f);
            vertices.push_back(0.0f);
            vertices.push_back(z * 1.5f);

            vertices.push_back((float)x / (width - 1));
            vertices.push_back((float)z / (length - 1));

            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            if (x == width - 1 || z == length - 1) continue;

            indices.push_back(x + z * width);
            indices.push_back(x + z * width + 1);
            indices.push_back(x + z * width + width);

            indices.push_back(x + z * width + 1);
            indices.push_back(x + z * width + width + 1);
            indices.push_back(x + z * width + width);
        }
    }

    return { vertices, indices };
}