#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "Opengl/VertexArray.h"
#include "Opengl/IndexBuffer.h"
#include "Opengl/Shader.h"
#include "Opengl/Texture.h"

struct Mesh {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

class Model
{
private:
	Mesh m_Mesh;

	Texture m_Texture;

	VertexArray m_VertexArray;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;

public:
	Model(const std::string& objPath, const std::string& texturePath, const std::string& shaderPath);
	Model(Mesh mesh, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void UpdateGeometry(Mesh mesh);

	void Draw(const Renderer& renderer, const glm::vec3 position, const glm::vec3 rotation);
};

void LoadOBJ(const std::string& filepath, std::vector<float>& vertices, std::vector<unsigned int>& indices);