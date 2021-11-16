#pragma once

class FrameBuffer
{
private:
	unsigned int m_RendererID;

	unsigned int m_Width;
	unsigned int m_Height;

	std::vector<GLenum> m_DrawBuffers;

public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();

	void AddColorTexture(unsigned int slot, unsigned int texWidth, unsigned int texHeight, GLenum colorAttachment);
	void AddDepthTexture(unsigned int slot, unsigned int texWidth, unsigned int texHeight);

	void Bind() const;
	void Unbind() const;
};
