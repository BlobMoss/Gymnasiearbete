#pragma once

#include "../Renderer.h"

#include "../opengl/VertexArray.h"
#include "../opengl/IndexBuffer.h"
#include "../opengl/Shader.h"
#include "../opengl/Texture.h"

#include "../Camera.h"

class Model
{
private:
	Mesh m_Mesh;

public:
	Texture m_Texture;

	VertexArray m_VertexArray;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;

	std::string m_ObjPath, m_TexturePath, m_ShaderPath;

	Model(const std::string& objPath, const std::string& texturePath, const std::string& shaderPath);
	Model(Mesh& mesh, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void UpdateData(Mesh& mesh);

	void Draw(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale);
};

Mesh LoadOBJ(const std::string& filepath);