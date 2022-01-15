#include "Renderer.h"

GLFWwindow* Renderer::m_Window;
GLFWmonitor* Renderer::m_Monitor;
const GLFWvidmode* Renderer::m_Mode;

unsigned int Renderer::m_WindowWidth;
unsigned int Renderer::m_WindowHeight;

unsigned int Renderer::pixelSize = 1;

// Resolution of full screen
unsigned int Renderer::monitorWidth;
unsigned int Renderer::monitorHeight;

bool Renderer::fullscreen = false;

// Variables to store while fullscreen
unsigned int Renderer::windowWidth;
unsigned int Renderer::windowHeight;
unsigned int Renderer::windowX;
unsigned int Renderer::windowY;

Renderer::Renderer()
{
    //Would be great to have screen drawing in here...
}
Renderer::~Renderer()
{

}

void Renderer::Init(GLFWwindow* window, GLFWmonitor* monitor, const GLFWvidmode* mode)
{
    m_Window = window;
    m_Monitor = monitor;
    m_Mode = mode;

    monitorWidth = m_Mode->width;
    monitorHeight = m_Mode->height;

    m_WindowWidth = referenceWidth * pixelSize;
    m_WindowHeight = referenceHeight * pixelSize;
    windowWidth = m_WindowWidth;
    windowHeight = m_WindowHeight;
    // Position window at centre of screen
    windowX = (monitorWidth -windowWidth) / 2;
    windowY = (monitorHeight - windowHeight) / 2;

    glfwSetWindowPos(m_Window, windowX, windowY);

    // Set callback functions
    glfwSetWindowSizeCallback(m_Window, window_size_callback);

    glfwSetWindowPosCallback(m_Window, window_pos_callback);

    glfwSetWindowSizeLimits(window, referenceWidth, referenceHeight, monitorWidth, monitorHeight);
}

void Renderer::ToggleFullscreen()
{
    fullscreen = !fullscreen;
    ToggleFullscreen(fullscreen);
}
void Renderer::ToggleFullscreen(bool full)
{
    if (full)
    {
        // Enter fullscreen
        glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, monitorWidth, monitorHeight, m_Mode->refreshRate);
    }
    else
    {
        // Enter windowed
        glfwSetWindowMonitor(m_Window, NULL, 0, 0, m_WindowWidth, m_WindowHeight, m_Mode->refreshRate);
        glfwSetWindowPos(m_Window, windowX, windowY);
    }
}

void Renderer::window_size_callback(GLFWwindow* window, int width, int height)
{
    if (!fullscreen)
    {
        m_WindowWidth = width;
        m_WindowHeight = height;
    }

    windowWidth = width;
    windowHeight = height;
    unsigned int resX = windowWidth / referenceWidth;
    unsigned int resY = windowHeight / referenceHeight;
    pixelSize = glm::min(resX, resY);
}

void Renderer::window_pos_callback(GLFWwindow* window, int xpos, int ypos)
{
    if (!fullscreen)
    {
        windowX = xpos;
        windowY = ypos;
    }
}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

    va.Unbind();
    ib.Unbind();
    shader.Unbind();
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}