#include "Image.h"

#include "../opengl/VertexBuffer.h"
#include "../opengl/VertexBufferLayout.h"
#include "../opengl/Shader.h"

Image::Image(const std::string& texturePath)
    : m_SortingOrder(0.5f), m_TexturePath(texturePath),
    m_VertexArray(), m_IndexBuffer(), m_Texture(texturePath), m_Shader("res/shaders/screen.shader")
{
    std::vector<float> ImageVertices = {
       0.0f                       , 0.0f                        , 0.0f, 0.0f,
       (float)m_Texture.GetWidth(), 0.0f                        , 1.0f, 0.0f,
       (float)m_Texture.GetWidth(), (float)m_Texture.GetHeight(), 1.0f, 1.0f,
       0.0f                       , (float)m_Texture.GetHeight(), 0.0f, 1.0f
    };

    std::vector<unsigned int> ImageIndices = {
        0, 1, 2,
        2, 3, 0
    };

    m_Mesh = Mesh{ ImageVertices, ImageIndices };

    UpdateData(m_Mesh);
}

Image::~Image()
{

}

void Image::UpdateData(Mesh& mesh)
{
    m_Mesh = mesh;

    VertexBuffer vb(&m_Mesh.vertices[0], m_Mesh.vertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2); // Vertex Position
    layout.Push<float>(2); // Texture Coordinate
    m_VertexArray.SetLayout(layout);

    m_IndexBuffer.SetData(&m_Mesh.indices[0], m_Mesh.indices.size());

    m_VertexArray.Unbind();
}

void Image::Draw(const glm::vec2 position)
{
    // Projection Matrix:
    glm::mat4 projMat = glm::ortho(0.0f, (float)referenceWidth, 0.0f, (float)referenceHeight, -1.0f, 1.0f);

    // View Matrix:
    glm::mat4 viewMat = glm::mat4(1.0f);

    // Model Matrix:
    glm::mat4 modelMat = glm::mat4(1.0f);

    modelMat = glm::translate(modelMat, glm::vec3(position.x, position.y, m_SortingOrder));

    // MVP
    glm::mat4 mvp = projMat * viewMat * modelMat;

    // Set uniforms of lit shader
    m_Shader.Bind();
    m_Texture.Bind(0);

    m_Shader.SetUniform1i("u_Texture", 0);
    m_Shader.SetUniformMat4f("u_MVP", mvp);

    // Draw model
    Renderer::DrawElements(m_VertexArray, m_IndexBuffer, m_Shader);
}

unsigned int Image::GetWidth()
{
    return m_Texture.GetWidth();
}
unsigned int Image::GetHeight()
{
    return m_Texture.GetHeight();
}
