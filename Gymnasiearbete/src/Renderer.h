#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include "FrameBuffer.h"

const unsigned int referenceWidth = 640;
const unsigned int referenceHeight = 360;
static unsigned int pixelSize = 2;

const float near = 1.0f;
const float far = 80.0f;

const unsigned int quadIndices[] = {
    0, 1, 2,
    2, 3, 0
};

class Renderer
{
private:
    FrameBuffer m_SpriteFrameBuffer;
    FrameBuffer m_ScreenFrameBuffer;

    VertexArray m_ScreenVertexArray;
    IndexBuffer m_ScreenIndexBuffer;
    Shader m_SpriteShader;
    Shader m_ScreenShader;

public:
    Renderer();
    ~Renderer();
    void BindSpriteFrameBuffer();
    void DrawScreen();
    void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
};