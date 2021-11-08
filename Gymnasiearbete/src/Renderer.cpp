#include "Renderer.h"

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

Renderer::Renderer()
    : m_SpriteFrameBuffer(), m_ScreenFrameBuffer(),
    m_ScreenVertexArray(), m_ScreenIndexBuffer(&quadIndices[0], sizeof(quadIndices) / sizeof(*quadIndices)),
    m_SpriteShader("res/shaders/toon.shader"), m_ScreenShader("res/shaders/screen.shader")
{
    m_SpriteFrameBuffer.AddColorTexture(1, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT0);
    m_SpriteFrameBuffer.AddColorTexture(2, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT1);
    m_SpriteFrameBuffer.AddDepthTexture(3, referenceWidth * 2, referenceHeight * 2);

    GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, DrawBuffers);

    m_ScreenFrameBuffer.AddColorTexture(4, referenceWidth, referenceHeight, GL_COLOR_ATTACHMENT0);
    

    float screenVertices[] = {
        0.0f          , 0.0f           , 0.0f, 0.0f,
        referenceWidth, 0.0f           , 1.0f, 0.0f,
        referenceWidth, referenceHeight, 1.0f, 1.0f,
        0.0f          , referenceHeight, 0.0f, 1.0f
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexBuffer vb(&screenVertices[0], sizeof(screenVertices));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_ScreenVertexArray.AddBuffer(vb, layout);


    m_SpriteShader.Bind();

    m_SpriteShader.SetUniform1i("u_ColorTexture", 1);

    m_SpriteShader.SetUniform1i("u_NormalTexture", 2);

    m_SpriteShader.SetUniform1i("u_DepthTexture", 3);

    m_SpriteShader.SetUniform1i("u_TexWidth", referenceWidth * 2);
    m_SpriteShader.SetUniform1i("u_TexHeight", referenceHeight * 2);

    m_SpriteShader.SetUniform1f("u_Near", near);
    m_SpriteShader.SetUniform1f("u_Far", far);

    glm::mat4 projMat = glm::ortho(0.0f, (float)referenceWidth * 2, 0.0f, (float)referenceHeight * 2, -1.0f, 1.0f);
    m_SpriteShader.SetUniformMat4f("u_MVP", projMat);


    m_ScreenShader.Bind();

    m_ScreenShader.SetUniform1i("u_Texture", 4);

    projMat = glm::ortho(0.0f, (float)referenceWidth, 0.0f, (float)referenceHeight, -1.0f, 1.0f);
    m_ScreenShader.SetUniformMat4f("u_MVP", projMat);
}
Renderer::~Renderer()
{

}

void Renderer::BindSpriteFrameBuffer()
{
    m_SpriteFrameBuffer.Bind();
    glViewport(0, 0, referenceWidth * 2, referenceHeight * 2);
    Clear();
}
void Renderer::DrawScreen()
{
    m_ScreenFrameBuffer.Bind();
    glViewport(0, 0, referenceWidth * pixelSize, referenceHeight * pixelSize);
    Clear();

    DrawElements(m_ScreenVertexArray, m_ScreenIndexBuffer, m_SpriteShader);

    m_ScreenFrameBuffer.Unbind();
    glViewport(0, 0, referenceWidth * pixelSize, referenceHeight * pixelSize);
    Clear();

    DrawElements(m_ScreenVertexArray, m_ScreenIndexBuffer, m_ScreenShader);
}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

    va.Unbind();
    ib.Unbind();
    shader.Unbind();
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}