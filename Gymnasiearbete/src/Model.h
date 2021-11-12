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
	Model(OBJSource source, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void Draw(Renderer renderer, glm::vec3 position, glm::vec3 rotation);
};

OBJSource LoadOBJ(const char* filepath);
