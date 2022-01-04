#include "Water.h"

Water::Water()
{
	m_Mesh = GenerateMesh();

	m_Model = new Model(m_Mesh, "res/textures/water_texture.png", "res/shaders/water.shader");
}
Water::~Water()
{

}

void Water::Update(float deltaTime)
{
    memset(m_Loaded, 0, sizeof(m_Loaded[0][0]) * 256 * 256);

    glm::vec3 pos = Camera::GetFollowTarget()->GetPosition();
    glm::vec2 chunkPos((int)(pos.x / 17.999f) + 128, (int)(pos.z / 17.999f) + 128);

    for (int x = chunkPos.x - 4; x < chunkPos.x + 4; x++)
    {
        for (int z = chunkPos.y - 4; z < chunkPos.y + 4; z++)
        {
            m_Loaded[x][z] = true;
        }
    }

    Sprite::Update(deltaTime);
}

void Water::Draw()
{
    for (int z = 0; z < 256; z++)
    {
        for (int x = 0; x < 256; x++)
        {
            if (m_Loaded[x][z])
            {
                m_Model->m_Shader.Bind();
                m_Model->m_Shader.SetUniform1f("u_Time", (float)glfwGetTime());
                glm::vec3 offset((x - 128.0f) * 17.999f - 8, 0.0f, (z - 128.0f) * 17.999f - 8);
                m_Model->m_Shader.SetUniform4f("u_Offset", offset.x, 0.0f, offset.z, 0.0f);
                m_Model->Draw(offset, m_Rotation, m_Scale);
            }
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
            vertices.push_back(x * 1.5);
            vertices.push_back(0.0f);
            vertices.push_back(z * 1.5);

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