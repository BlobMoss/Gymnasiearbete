#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "openGL/VertexBuffer.h"
#include "openGL/VertexBufferLayout.h"
#include "openGL/FrameBuffer.h"

#include "SpriteManager.h"
#include "Input.h"
#include "BlockGroup.h"
#include "Player.h"

int main(void)
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    
    GLFWwindow* window = glfwCreateWindow(referenceWidth * Renderer::pixelSize, referenceHeight * Renderer::pixelSize, "Gymnasiearbete", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    Renderer::Init(window, monitor, mode);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW is not OK" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glClearColor(0.25f, 0.20f, 0.20f, 1.00f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Render only faces facing camera
    glEnable(GL_CULL_FACE);


    FrameBuffer spriteFrameBuffer(referenceWidth * 2, referenceHeight * 2);
    spriteFrameBuffer.AddColorTexture(1, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT0);
    spriteFrameBuffer.AddColorTexture(2, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT1);
    spriteFrameBuffer.AddDepthTexture(3, referenceWidth * 2, referenceHeight * 2);

    FrameBuffer screenFrameBuffer(referenceWidth * 2, referenceHeight * 2);
    screenFrameBuffer.AddColorTexture(4, referenceWidth, referenceHeight, GL_COLOR_ATTACHMENT0);


    // Set up screen shape:
    // I do not think adding + 0.01 here is the correct solution
    float screenVertices[] = {
        0.0f                  , 0.0f                   , 0.0f, 0.0f,
        referenceWidth + 0.01f, 0.0f                   , 1.0f, 0.0f,
        referenceWidth + 0.01f, referenceHeight + 0.01f, 1.0f, 1.0f,
        0.0f                  , referenceHeight + 0.01f, 0.0f, 1.0f
    };

    unsigned int screenIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    VertexArray screenVertexArray;
    IndexBuffer screenIndexBuffer;
    Shader toonShader("res/shaders/toon.shader");
    Shader screenShader("res/shaders/screen.shader");

    VertexBuffer vb(&screenVertices[0], sizeof(screenVertices));

    VertexBufferLayout layout;
    layout.Push<float>(2); // Vertex Position
    layout.Push<float>(2); // Texture Coordinate
    screenVertexArray.AddBuffer(vb, layout);

    screenIndexBuffer.SetData(&screenIndices[0], sizeof(screenIndices) / sizeof(*screenIndices));

    // Set toon shader uniforms
    toonShader.Bind();

    toonShader.SetUniform1i("u_ColorTexture", 1);
    toonShader.SetUniform1i("u_NormalTexture", 2);
    toonShader.SetUniform1i("u_DepthTexture", 3);

    toonShader.SetUniform1i("u_TexWidth", referenceWidth * 2);
    toonShader.SetUniform1i("u_TexHeight", referenceHeight * 2);

    toonShader.SetUniform1f("u_Near", near);
    toonShader.SetUniform1f("u_Far", far);

    toonShader.SetUniformMat4f("u_MVP", glm::ortho(0.0f, (float)referenceWidth * 2, 0.0f, (float)referenceHeight * 2, -1.0f, 1.0f));


    // Set screen shader uniforms
    screenShader.Bind();

    screenShader.SetUniform1i("u_Texture", 4);

    screenShader.SetUniformMat4f("u_MVP", glm::ortho(0.0f, (float)referenceWidth, 0.0f, (float)referenceHeight, -1.0f, 1.0f));

    Input input(window);

    SpriteManager spriteManager;

    {

    Player* player = new Player();
    spriteManager.AddSprite(player);

    player->SetPosition(glm::vec3(-9.0f, 0.0f, 0.0f));
    player->SetScale(glm::vec3(0.6f));

    Camera::SetFollowTarget(*player);

    Sprite* teapot = new Sprite(new Model("res/models/teapot.obj", "res/textures/teapot_texture.png", "res/shaders/lighting.shader"));
    spriteManager.AddSprite(teapot);

    teapot->SetPosition(glm::vec3(9.0, -1.0, 0.0));

    BlockGroup* blockGroup = new BlockGroup();
    spriteManager.AddSprite(blockGroup);

    blockGroup->SetPosition(glm::vec3(0.0, 0.0, 0.0));
    blockGroup->SetBlock(glm::ivec3(18, 0, 15), PLANKS);
    blockGroup->SetBlock(glm::ivec3(18, 0, 16), PLANKS);
    blockGroup->SetBlock(glm::ivec3(18, 0, 17), PLANKS);
    blockGroup->SetBlock(glm::ivec3(17, 0, 17), PLANKS);
    blockGroup->SetBlock(glm::ivec3(16, 0, 17), PLANKS);
    blockGroup->SetBlock(glm::ivec3(17, 0, 16), PLANKS);
    blockGroup->SetBlock(glm::ivec3(17, 0, 15), PLANKS);
    blockGroup->SetBlock(glm::ivec3(16, 0, 16), PLANKS);
    blockGroup->SetBlock(glm::ivec3(15, 0, 16), PLANKS);
    blockGroup->SetBlock(glm::ivec3(16, 0, 15), PLANKS);

    blockGroup->SetBlock(glm::ivec3(18, 1, 15), PLANKS);
    blockGroup->SetBlock(glm::ivec3(18, 1, 16), PLANKS);
    blockGroup->SetBlock(glm::ivec3(18, 1, 17), PLANKS);

    }

    // Keep track of time to calculate time delta
    float lastElapsedTime = 0.0f;
    float elapsedTime = 0.0f;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        if (Input::KeyDown(KEY_ESCAPE)) glfwSetWindowShouldClose(window, true);

        if (Input::KeyDown(KEY_F)) Renderer::ToggleFullscreen();


        // Updating:
        elapsedTime = (float)glfwGetTime();
        float deltaTime = elapsedTime - lastElapsedTime;
        lastElapsedTime = elapsedTime;

        // Update camera
        Camera::Update(deltaTime); 
        // Update sprites
        spriteManager.Update(deltaTime);
        // Update input arrays 
        input.Update(deltaTime);

        // Drawing:
        // Bind sprite framebuffer
        spriteFrameBuffer.Bind();
        glClearColor(0.25f, 0.20f, 0.20f, 1.00f);
        Renderer::Clear();

        // Draw sprites on that framebuffer
        spriteManager.Draw();

        // Bind screen framebuffer
        screenFrameBuffer.Bind();
        Renderer::Clear();

        // Draw sprite framebuffer with toon shader to screen framebuffer
        Renderer::DrawElements(screenVertexArray, screenIndexBuffer, toonShader);

        // Unbind framebuffers
        screenFrameBuffer.Unbind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        Renderer::Clear();

        int offsetX = (Renderer::windowWidth - (referenceWidth * Renderer::pixelSize)) / 2;
        int offsetY = (Renderer::windowHeight - (referenceHeight * Renderer::pixelSize)) / 2;
        glViewport(offsetX, offsetY, Renderer::windowWidth - offsetX * 2, Renderer::windowHeight - offsetY * 2);

        // Draw screen framebuffer to screen
        Renderer::DrawElements(screenVertexArray, screenIndexBuffer, screenShader);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}