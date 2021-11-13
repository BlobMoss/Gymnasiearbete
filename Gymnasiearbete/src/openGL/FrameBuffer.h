#pragma once

class FrameBuffer
{
private:
	unsigned int m_RendererID;

	std::vector<GLenum> m_DrawBuffers;

public:
	FrameBuffer();
	~FrameBuffer();

	void AddColorTexture(unsigned int slot, unsigned int texWidth, unsigned int texHeight, GLenum colorAttachment);
	void AddDepthTexture(unsigned int slot, unsigned int texWidth, unsigned int texHeight);

	void Bind() const;
	void Unbind() const;
};
