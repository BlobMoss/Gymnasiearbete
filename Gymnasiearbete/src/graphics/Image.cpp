#include "Image.h"

#include "../opengl/VertexBuffer.h"
#include "../opengl/VertexBufferLayout.h"
#include "../opengl/Shader.h"

Image::Image(const std::string& texturePath)
    : m_SortingOrder(0.5f), m_TexturePath(texturePath),
    m_VertexArray(), m_VertexBuffer(), m_IndexBuffer(), m_Texture(texturePath), m_Shader("res/shaders/screen.shader")
{
    m_Width = m_Texture.GetWidth();
    m_Height = m_Texture.GetHeight();

    std::vector<float> ImageVertices = {
       0.0f          , 0.0f           , 0.0f, 0.0f,
       (float)m_Width, 0.0f           , 1.0f, 0.0f,
       (float)m_Width, (float)m_Height, 1.0f, 1.0f,
       0.0f          , (float)m_Height, 0.0f, 1.0f
    };

    std::vector<unsigned int> ImageIndices = {
        0, 1, 2,
        2, 3, 0
    };

    m_Mesh = Mesh{ ImageVertices, ImageIndices };

    UpdateData(m_Mesh);
}

Image::Image(const std::string& texturePath, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
    : m_SortingOrder(0.5f), m_TexturePath(texturePath),
    m_VertexArray(), m_VertexBuffer(), m_IndexBuffer(), m_Texture(texturePath), m_Shader("res/shaders/screen.shader")
{
    m_Width = width;
    m_Height = height;

    std::vector<float> ImageVertices = {
       0.0f          , 0.0f           , x / (float)m_Texture.GetWidth()            , y / (float)m_Texture.GetHeight(),
       (float)m_Width, 0.0f           , (x + m_Width) / (float)m_Texture.GetWidth(), y / (float)m_Texture.GetHeight(),
       (float)m_Width, (float)m_Height, (x + m_Width) / (float)m_Texture.GetWidth(), (y + m_Height) / (float)m_Texture.GetHeight(),
       0.0f          , (float)m_Height, x / (float)m_Texture.GetWidth()            , (y + m_Height) / (float)m_Texture.GetHeight()
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

    if (mesh.vertices.size() == 0 || mesh.indices.size() == 0) return;

    m_VertexBuffer.SetData(&m_Mesh.vertices[0], m_Mesh.vertices.size() * sizeof(float));
    m_IndexBuffer.SetData(&m_Mesh.indices[0], m_Mesh.indices.size());

    VertexBufferLayout layout;
    layout.Push<float>(2); // Vertex Position
    layout.Push<float>(2); // Texture Coordinate
    m_VertexArray.SetLayout(layout);
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
    return m_Width;
}
unsigned int Image::GetHeight()
{
    return m_Height;
}
