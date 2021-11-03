#pragma once

#include "Sprite.h"

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

class Model : public Sprite
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;

	Texture m_Texture;

	VertexArray m_VertexArray;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;
public:
	Model(OBJSource source, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void Update(float deltaTime) override;

	void Draw(Renderer renderer) override;

	void SetPosition(glm::vec3 p);
	glm::vec3 GetPosition();
	void SetRotation(glm::vec3 r);
	glm::vec3 GetRotation();
};

OBJSource LoadOBJ(const char* filepath);
