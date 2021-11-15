#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "openGL/VertexArray.h"
#include "openGL/IndexBuffer.h"
#include "openGL/Shader.h"

const unsigned int referenceWidth = 640;
const unsigned int referenceHeight = 360;
static unsigned int pixelSize = 2;

const float near = 1.0f;
const float far = 100.0f;

class Renderer
{
private:

public:
    Renderer();
    ~Renderer();

    static void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    static void Clear();
};

