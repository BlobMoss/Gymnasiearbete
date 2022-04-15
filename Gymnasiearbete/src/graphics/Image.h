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

	Texture m_Texture;

	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;

	unsigned int m_Width, m_Height;

public:
	std::string m_TexturePath;

public:
	Image(const std::string& texturePath);
	Image(const std::string& texturePath, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	~Image();

	void UpdateData(Mesh& mesh);

	void Draw(const glm::vec2 position, const float sortingOrder);

	unsigned int GetWidth();
	unsigned int GetHeight();
};

