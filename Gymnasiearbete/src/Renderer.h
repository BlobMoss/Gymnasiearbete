#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

const unsigned int referenceWidth = 640;
const unsigned int referenceHeight = 360;
static unsigned int pixelSize = 2;

const float near = 1.0f;
const float far = 80.0f;


class Renderer
{
public:
    void Clear() const;
    void DrawScreen();
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};