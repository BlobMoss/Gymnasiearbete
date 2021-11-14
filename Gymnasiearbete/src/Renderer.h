#pragma once

#include <GL/glew.h>
#include <iostream>

#include "openGL/VertexArray.h"
#include "openGL/IndexBuffer.h"
#include "openGL/Shader.h"

const unsigned int referenceWidth = 640;
const unsigned int referenceHeight = 360;
static unsigned int pixelSize = 3;

const float near = 1.0f;
const float far = 100.0f;

class Renderer
{
private:

public:
    Renderer();
    ~Renderer();
};

static void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

static void Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}