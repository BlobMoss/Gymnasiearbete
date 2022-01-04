#pragma once

#include "Common.h"

#include "openGL/VertexArray.h"
#include "openGL/IndexBuffer.h"
#include "openGL/Shader.h"

// Pixel resolution
const unsigned int referenceWidth = 640;
const unsigned int referenceHeight = 360;

// Projection
const float nearPlane = 40.0f;
const float farPlane = 95.0f;

struct Mesh {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

class Renderer
{
private:
    static GLFWwindow* m_Window;
    static GLFWmonitor* m_Monitor;
    static const GLFWvidmode* m_Mode;

    // Screen dimensions stored while in fullscreen
    static unsigned int m_WindowWidth;
    static unsigned int m_WindowHeight;

public:
    static unsigned int pixelSize;

    // Resolution of full screen
    static unsigned int monitorWidth;
    static unsigned int monitorHeight;

    static bool fullscreen;

    // Screen rectangle variables
    static unsigned int windowWidth;
    static unsigned int windowHeight;
    static unsigned int windowX;
    static unsigned int windowY;

    Renderer();
    ~Renderer();

    static void Init(GLFWwindow* window, GLFWmonitor* monitor, const GLFWvidmode* mode);

    static void ToggleFullscreen();
    static void ToggleFullscreen(bool full);

    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void window_pos_callback(GLFWwindow* window, int xpos, int ypos);

    static void DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
    static void Clear();
};

