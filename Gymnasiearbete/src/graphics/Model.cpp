#include "Model.h"

#include "../opengl/VertexBuffer.h"
#include "../opengl/VertexBufferLayout.h"
#include "../opengl/Shader.h"

#include "../gameplay/SpriteManager.h"

glm::mat4 Model::projMat = glm::perspective(glm::radians(30.0f), (GLfloat)referenceWidth / (GLfloat)referenceHeight, nearPlane, farPlane);
glm::mat4 Model::viewMat = glm::mat4(1.0f);
glm::mat4 Model::lightSpaceMat = glm::mat4(1.0f);

Model::Model(const std::string& objPath, const std::string& texturePath, const std::string& shaderPath)
    : m_ObjPath(objPath), m_TexturePath(texturePath), m_ShaderPath(shaderPath),
    m_VertexArray(), m_VertexBuffer(), m_IndexBuffer(), m_Texture(texturePath), m_Shader(shaderPath), m_ShadowShader("res/shaders/shadow_mapping.shader"), 
    m_CastsShadow(true), m_AmbientStrength(0.3f), m_DiffuseStrength(1.0f), m_SpecularStrength(0.6f), m_HighlightColor(glm::vec4(1.0f, 1.0f, 0.8f, 1.0f))
{
    UpdateData(LoadOBJ(objPath));
}

Model::Model(Mesh mesh, const std::string& texturePath, const std::string& shaderPath)
    : m_VertexArray(), m_VertexBuffer(), m_IndexBuffer(), m_Texture(texturePath), m_Shader(shaderPath), m_ShadowShader("res/shaders/shadow_mapping.shader"),
    m_CastsShadow(true), m_AmbientStrength(0.3f), m_DiffuseStrength(1.0f), m_SpecularStrength(0.6f), m_HighlightColor(glm::vec4(1.0f, 1.0f, 0.8f, 1.0f))
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

    m_Shader.Bind();
    m_Texture.Bind(0);

    m_Shader.SetUniform1i("u_Texture", 0);

    m_Shader.SetUniformMat4f("u_ProjectionMatrix", projMat);

    // Lighting
    m_Shader.SetUniform3f("u_LightColor", 1.0, 0.95f, 0.9f);
    m_Shader.SetUniform1f("u_AmbientStrength", m_AmbientStrength);
    m_Shader.SetUniform1f("u_DiffuseStrength", m_DiffuseStrength);
    m_Shader.SetUniform1f("u_SpecularStrength", m_SpecularStrength);
}

void Model::Draw(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale, const glm::vec4 color, const bool highLighted)
{
    if (!SpriteManager::drawingShadows)
    {
        // Model Matrix:
        glm::mat4 modelMat = glm::mat4(1.0f);

        modelMat = glm::translate(modelMat, position);
        modelMat = glm::scale(modelMat, scale);
        modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1, 0));
        modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1, 0, 0));
        modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1));

        // Set uniforms
        m_Shader.Bind();
        m_Texture.Bind(0);

        m_Shader.SetUniform1i("u_ShadowMap", 6);

        // Matrices
        m_Shader.SetUniformMat4f("u_ModelMatrix", modelMat);
        m_Shader.SetUniformMat4f("u_ViewMatrix", viewMat);
        m_Shader.SetUniformMat4f("u_LightSpaceMatrix", lightSpaceMat);

        // Color
        m_Shader.SetUniform4f("u_Color", color.r, color.g, color.b, color.a);

        glm::vec3 lightPos = glm::vec3(20.0f, 30.0f, 0.0f) + Camera::m_TargetPosition;
        m_Shader.SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z); // Position of sun
        m_Shader.SetUniform3f("u_ViewPos", Camera::m_Position.x, Camera::m_Position.y, Camera::m_Position.z);

        // Highlighting
        if (highLighted)
            m_Shader.SetUniform4f("u_HighlightColor", m_HighlightColor.r, m_HighlightColor.g, m_HighlightColor.b, m_HighlightColor.a);
        else
            m_Shader.SetUniform4f("u_HighlightColor", 0.0f, 0.0f, 0.0f, 1.0f);

        // Draw model
        Renderer::DrawElements(m_VertexArray, m_IndexBuffer, m_Shader);
    }
    else if (m_CastsShadow)
    {
        // Model Matrix:
        glm::mat4 modelMat = glm::mat4(1.0f);

        modelMat = glm::translate(modelMat, position);
        modelMat = glm::scale(modelMat, scale);
        modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1, 0, 0));
        modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0, 1, 0));
        modelMat = glm::rotate(modelMat, rotation.z, glm::vec3(0, 0, 1));

        // Set uniforms of lit shader
        m_ShadowShader.Bind();

        m_ShadowShader.SetUniformMat4f("u_LightSpaceMatrix", lightSpaceMat);
        m_ShadowShader.SetUniformMat4f("u_ModelMatrix", modelMat);

        // Draw model
        Renderer::DrawElements(m_VertexArray, m_IndexBuffer, m_ShadowShader);
    }
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