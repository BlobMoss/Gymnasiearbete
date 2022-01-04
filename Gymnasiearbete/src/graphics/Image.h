#pragma once
#include "../Renderer.h"

#include "../opengl/VertexArray.h"
#include "../opengl/IndexBuffer.h"
#include "../opengl/Shader.h"
#include "../opengl/Texture.h"

#include "../Camera.h"

class Image
{
protected:
	Mesh m_Mesh;

	float m_SortingOrder;

	Texture m_Texture;

	VertexArray m_VertexArray;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;

public:
	std::string m_TexturePath;

	Image(const std::string& texturePath);
	~Image();

	void UpdateData(Mesh& mesh);

	void Draw(const glm::vec2 position);

	unsigned int GetWidth();
	unsigned int GetHeight();
};

