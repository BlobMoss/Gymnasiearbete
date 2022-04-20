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

	std::string m_ObjPath;
	std::string m_TexturePath;
	std::string m_ShaderPath;

	static glm::mat4 projMat;
	static glm::mat4 viewMat;
	static glm::mat4 lightSpaceMat;

	Texture m_Texture;
	bool m_HasTransparency;
	bool m_CastsShadow;
	float m_AmbientStrength;
	float m_DiffuseStrength;
	float m_SpecularStrength;
	glm::vec4 m_HighlightColor;

	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	Shader m_Shader;
	Shader m_ShadowShader;

public:
	Model(const std::string& objPath, const std::string& texturePath, const std::string& shaderPath);
	Model(Mesh mesh, const std::string& texturePath, const std::string& shaderPath);
	~Model();

	void UpdateData(Mesh mesh);

	void Draw(const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale, const glm::vec4 color, const bool highlighted);
};

Mesh LoadOBJ(const std::string& filepath);