#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

struct OBJSource
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

class Model
{
private:
	Texture m_Texture;

	VertexArray m_VertexArray;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;

public:
	Model(const OBJSource source, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void Draw(const Renderer &renderer, const glm::vec3 position, const glm::vec3 rotation);
};

OBJSource LoadOBJ(const char* filepath);
