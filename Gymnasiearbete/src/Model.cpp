#include "Model.h"

#include "Opengl/VertexBuffer.h"
#include "Opengl/VertexBufferLayout.h"
#include "Opengl/Shader.h"

Model::Model(const std::string& objPath, const std::string& texturePath, const std::string& shaderPath)
    : m_Mesh(LoadOBJ(objPath)), m_VertexArray(), m_IndexBuffer(), m_Texture(texturePath), m_Shader(shaderPath)
{
    UpdateData(m_Mesh);
}

Model::Model(Mesh& mesh, const std::string& texturePath, const std::string& shaderPath)
    : m_Mesh(mesh), m_VertexArray(), m_IndexBuffer(), m_Texture(texturePath), m_Shader(shaderPath)
{
    UpdateData(m_Mesh);
}

Model::~Model()
{
    
}

void Model::UpdateData(Mesh& mesh)
{
    m_Mesh = mesh;

    VertexBuffer vb(&m_Mesh.vertices[0], m_Mesh.vertices.size() * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(3); // Vertex Position
    layout.Push<float>(2); // Texture Coordinate
    layout.Push<float>(3); // Vertex Normal
    m_VertexArray.AddBuffer(vb, layout);

    m_IndexBuffer.SetData(&m_Mesh.indices[0], m_Mesh.indices.size());

    vb.Unbind();
    m_VertexArray.Unbind();
    m_IndexBuffer.Unbind();
}

void Model::Draw(const Renderer& renderer, const glm::vec3 position, const glm::vec3 rotation)
{
    // Projection Matrix:
    glm::mat4 projMat = glm::perspective(45.0f, (GLfloat)referenceWidth / (GLfloat)referenceHeight, near, far);

    // View Matrix:
    glm::mat4 viewMat = glm::mat4(1.0f);
    viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -15.0f));

    // Model Matrix:
    glm::mat4 modelMat = glm::mat4(1.0f);
    //
    modelMat = glm::translate(modelMat, position);
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
    m_Shader.SetUniform3f("u_LightPos", 10.0f, 0.0f, 20.0f);
    m_Shader.SetUniform3f("u_ViewPos", 0.0f, 0.0f, 10.0f);
    m_Shader.SetUniformMat4f("u_NormalMatrix", normalMat);

    // Draw model
    renderer.DrawElements(m_VertexArray, m_IndexBuffer, m_Shader);
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