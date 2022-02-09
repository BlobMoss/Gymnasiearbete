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

public:
	Mesh m_Mesh;

	Texture m_Texture;

	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;

public:
	Model(const std::string& objPath, const std::string& texturePath, const std::string& shaderPath);
	Model(Mesh mesh, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void UpdateData(Mesh mesh);

	void Draw(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale);
};

Mesh LoadOBJ(const std::string& filepath);