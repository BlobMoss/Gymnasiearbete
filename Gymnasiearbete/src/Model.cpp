#include "Model.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Shader.h"

#include <iostream> 

Model::Model(OBJSource source, const std::string& texturePath, const std::string& shaderPath)
    : m_Texture(texturePath),
      m_VertexArray(), m_IndexBuffer(&source.indices[0], source.indices.size()), m_Shader(shaderPath)
{
    VertexBuffer vb(&source.vertices[0], source.vertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);
    m_VertexArray.AddBuffer(vb, layout);

    m_Texture.Bind(0);
    m_Shader.Bind();
    m_Shader.SetUniform1i("u_Texture", 0);

    vb.Unbind();
    m_VertexArray.Unbind();
    m_IndexBuffer.Unbind();
    m_Texture.Unbind(0);
    m_Shader.Unbind();
}
Model::~Model()
{
    
}

void Model::Draw(const Renderer &renderer, const glm::vec3 position, const glm::vec3 rotation)
{
    //PROJECTION:
    glm::mat4 projMat = glm::perspective(45.0f, (GLfloat)referenceWidth / (GLfloat)referenceHeight, near, far);

    //VIEW:
    glm::mat4 viewMat = glm::mat4(1.0f);
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -15.0f));

    //MODEL:
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, position);
    modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1, 0, 0));
    modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1, 0));
    modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1));

    //MVP
    glm::mat4 mvp = projMat * viewMat * modelMat;

    //Normal Matrix
    glm::mat4 normalMat = glm::transpose(glm::inverse(modelMat));

    m_Shader.Bind();
    m_Texture.Bind(0);

    m_Shader.SetUniform1i("u_Texture", 0);
    m_Shader.SetUniformMat4f("u_MVP", mvp);
    m_Shader.SetUniformMat4f("u_ModelMatrix", modelMat);
    m_Shader.SetUniform3f("u_LightPos", 10.0f, 0.0f, 20.0f);
    m_Shader.SetUniform3f("u_ViewPos", 0.0f, 0.0f, 10.0f);
    m_Shader.SetUniformMat4f("u_NormalMatrix", normalMat);

    renderer.DrawElements(m_VertexArray, m_IndexBuffer, m_Shader);
}

OBJSource LoadOBJ(const char* filepath)
{
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    FILE* file;
    fopen_s(&file, filepath, "r");
    if (file == NULL)
    {
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

    return { vertices, indices };
}