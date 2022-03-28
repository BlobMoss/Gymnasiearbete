#include "Model.h"

#include "../opengl/VertexBuffer.h"
#include "../opengl/VertexBufferLayout.h"
#include "../opengl/Shader.h"

Model::Model(const std::string& objPath, const std::string& texturePath, const std::string& shaderPath)
    :m_VertexArray(), m_VertexBuffer(), m_IndexBuffer(), m_Texture(texturePath), m_Shader(shaderPath)
{
    UpdateData(LoadOBJ(objPath));
}

Model::Model(Mesh mesh, const std::string& texturePath, const std::string& shaderPath)
    : m_VertexArray(), m_VertexBuffer(), m_IndexBuffer(), m_Texture(texturePath), m_Shader(shaderPath)
{
    UpdateData(mesh);
}

Model::~Model()
{
    
}

void Model::UpdateData(Mesh mesh)
{
    if (mesh.vertices.size() == 0 || mesh.indices.size() == 0) return;

    m_Mesh = mesh;

    m_VertexBuffer.SetData(&m_Mesh.vertices[0], m_Mesh.vertices.size() * sizeof(float));
    m_IndexBuffer.SetData(&m_Mesh.indices[0], m_Mesh.indices.size());

    VertexBufferLayout layout;
    layout.Push<float>(3); // Vertex Position
    layout.Push<float>(2); // Texture Coordinate
    layout.Push<float>(3); // Vertex Normal
    m_VertexArray.SetLayout(layout);
}

void Model::Draw(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale)
{
    // Projection Matrix:
    glm::mat4 projMat = glm::perspective(glm::radians(30.0f), (GLfloat)referenceWidth / (GLfloat)referenceHeight, nearPlane, farPlane);

    // View Matrix:
    glm::mat4 viewMat = glm::mat4(1.0f);

    viewMat = glm::rotate(viewMat, Camera::m_Rotation.x, glm::vec3(1, 0, 0));
    viewMat = glm::rotate(viewMat, Camera::m_Rotation.y, glm::vec3(0, 1, 0));
    viewMat = glm::rotate(viewMat, Camera::m_Rotation.z, glm::vec3(0, 0, 1));
    viewMat = glm::translate(viewMat, -Camera::m_Position);

    // Model Matrix:
    glm::mat4 modelMat = glm::mat4(1.0f);

    modelMat = glm::translate(modelMat, position);
    modelMat = glm::scale(modelMat, scale);
    modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1, 0, 0));
    modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1, 0));
    modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1));

    // MVP
    glm::mat4 mvp = projMat * viewMat * modelMat;

    // Normal Matrix
    glm::mat4 normalMat = glm::transpose(glm::inverse(modelMat));

    // Set uniforms of lit shader
    m_Shader.Bind();
    m_Texture.Bind(0);

    m_Shader.SetUniform1i("u_Texture", 0);
    m_Shader.SetUniformMat4f("u_MVP", mvp);
    m_Shader.SetUniformMat4f("u_ModelMatrix", modelMat);
    m_Shader.SetUniform3f("u_LightPos", 20.0f, 40.0f, 20.0f);
    m_Shader.SetUniform3f("u_ViewPos", 0.0f, 0.0f, 10.0f);
    m_Shader.SetUniformMat4f("u_NormalMatrix", normalMat);
    m_Shader.SetUniform1i("u_Highlighted", m_Highlighted ? 1 : 0);

    // Draw model
    Renderer::DrawElements(m_VertexArray, m_IndexBuffer, m_Shader);
}

Mesh LoadOBJ(const std::string& filepath)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    FILE* file;
    fopen_s(&file, filepath.c_str(), "r");
    if (file == NULL)
    {
        std::cout << "Filepath: " << filepath << " does not exist!";
        return { vertices, indices };
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    unsigned int index = 0;

    while (true)
    {
        char header[128];

        int res = fscanf_s(file, "%127s", header, 128);
        if (res == EOF)
        {
            break;
        }

        if (strcmp(header, "v") == 0)
        {
            glm::vec3 p;
            (void)fscanf_s(file, "%f %f %f\n", &p.x, &p.y, &p.z);
            temp_positions.push_back(p);
        }
        else if (strcmp(header, "vt") == 0)
        {
            glm::vec2 uv;
            (void)fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        }
        else if (strcmp(header, "vn") == 0)
        {
            glm::vec3 n;
            (void)fscanf_s(file, "%f %f %f\n", &n.x, &n.y, &n.z);
            temp_normals.push_back(n);
        }
        else if (strcmp(header, "f") == 0)
        {
            unsigned int positionIndex[3], uvIndex[3], normalIndex[3];

            (void)fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &positionIndex[0], &uvIndex[0], &normalIndex[0], &positionIndex[1], &uvIndex[1], &normalIndex[1], &positionIndex[2], &uvIndex[2], &normalIndex[2]);

            for (unsigned int i = 0; i < 3; i++)
            {
                vertices.push_back(temp_positions[positionIndex[i] - 1].x);
                vertices.push_back(temp_positions[positionIndex[i] - 1].y);
                vertices.push_back(temp_positions[positionIndex[i] - 1].z);

                vertices.push_back(temp_uvs[uvIndex[i] - 1].x);
                vertices.push_back(temp_uvs[uvIndex[i] - 1].y);

                vertices.push_back(temp_normals[normalIndex[i] - 1].x);
                vertices.push_back(temp_normals[normalIndex[i] - 1].y);
                vertices.push_back(temp_normals[normalIndex[i] - 1].z);

                indices.push_back(index);
                index++;
            }
        }
    }

    fclose(file);

    return { vertices, indices };
}