#include "../Renderer.h"

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &m_RendererID);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::AddColorTexture(unsigned int slot, unsigned int texWidth, unsigned int texHeight, GLenum colorAttachment)
{
    // Create new texture
    unsigned int texture = 0;
    glGenTextures(1, &texture);

    // Bind texture to "slot"
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Texture paramaters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    Bind();

    // Attach texture to framebuffer at "colorAttachment"
    glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, texture, 0);

    // Add color attachment to a list
    m_DrawBuffers.push_back(colorAttachment);
    glDrawBuffers(m_DrawBuffers.size(), &m_DrawBuffers[0]);

    glActiveTexture(GL_TEXTURE0);
    Unbind();
}

void FrameBuffer::AddDepthTexture(unsigned int slot, unsigned int texWidth, unsigned int texHeight)
{
    // Create new texture
    unsigned int texture;
    glGenTextures(1, &texture);

    // Bind texture to "slot"
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Texture paramaters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, texWidth, texHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    Bind();

    // Attach texture to framebuffers depth attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

    glActiveTexture(GL_TEXTURE0);
    Unbind();
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}