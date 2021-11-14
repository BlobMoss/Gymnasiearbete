#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "Opengl/VertexArray.h"
#include "Opengl/IndexBuffer.h"
#include "Opengl/Shader.h"
#include "Opengl/Texture.h"

#include "Camera.h"

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
	Model(Mesh& mesh, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void UpdateData(Mesh& mesh);

	void Draw(Camera& camera, const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale);
};

Mesh LoadOBJ(const std::string& filepath);