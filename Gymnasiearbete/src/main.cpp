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

#include "SpriteManager.h"
#include "BlockGroup.h"

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(referenceWidth * pixelSize, referenceHeight * pixelSize, "Gymnasiearbete", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

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
    //glEnable(GL_CULL_FACE);

    Renderer renderer;

    FrameBuffer spriteFrameBuffer;
    spriteFrameBuffer.AddColorTexture(1, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT0);
    spriteFrameBuffer.AddColorTexture(2, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT1);
    spriteFrameBuffer.AddDepthTexture(3, referenceWidth * 2, referenceHeight * 2);

    FrameBuffer screenFrameBuffer;
    screenFrameBuffer.AddColorTexture(4, referenceWidth, referenceHeight, GL_COLOR_ATTACHMENT0);


    // Set up screen shape:
    float screenVertices[] = {
        0.0f          , 0.0f           , 0.0f, 0.0f,
        referenceWidth, 0.0f           , 1.0f, 0.0f,
        referenceWidth, referenceHeight, 1.0f, 1.0f,
        0.0f          , referenceHeight, 0.0f, 1.0f
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


    SpriteManager spriteManager;

    {

    Sprite* gem = new Sprite(new Model("res/models/gem.obj", "res/textures/gem_texture.png", "res/shaders/lighting.shader"));
    spriteManager.AddSprite(gem);

    gem->SetPosition(glm::vec3(-3.0, 0.0, -1.0));

    spriteManager.AddSprite(new Sprite(new Model("res/models/teapot.obj", "res/textures/teapot_texture.png", "res/shaders/lighting.shader")));

    BlockGroup* blockGroup = new BlockGroup();
    spriteManager.AddSprite(blockGroup);

    blockGroup->SetPosition(glm::vec3(8.0, 0.0, -1.0));
    blockGroup->SetBlock(glm::ivec3(17, 0, 17), SOLID);
    blockGroup->SetBlock(glm::ivec3(16, 0, 17), SOLID);
    blockGroup->SetBlock(glm::ivec3(16, 0, 16), SOLID);
    blockGroup->SetBlock(glm::ivec3(16, 1, 16), SOLID);
    blockGroup->SetBlock(glm::ivec3(15, 0, 16), SOLID);

    }

    // Keep track of time to calculate time delta
    float lastElapsedTime = 0.0f;
    float elapsedTime = 0.0f;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Updating:
        elapsedTime = (float)glfwGetTime();
        float deltaTime = elapsedTime - lastElapsedTime;
        lastElapsedTime = elapsedTime;

        // Update sprites
        spriteManager.Update(deltaTime);


        // Drawing:
        // Bind sprite framebuffer
        spriteFrameBuffer.Bind();
        glViewport(0, 0, referenceWidth * 2, referenceHeight * 2);
        renderer.Clear();

        // Draw sprites on that framebuffer
        spriteManager.Draw(renderer);

        // Bind screen framebuffer
        screenFrameBuffer.Bind();
        glViewport(0, 0, referenceWidth * 2, referenceHeight * 2);
        renderer.Clear();

        // Draw sprite framebuffer with toon shader to screen framebuffer
        renderer.DrawElements(screenVertexArray, screenIndexBuffer, toonShader);

        // Unbind framebuffers
        screenFrameBuffer.Unbind();
        glViewport(0, 0, referenceWidth * pixelSize, referenceHeight * pixelSize);
        renderer.Clear();

        // Draw screen framebuffer to screen
        renderer.DrawElements(screenVertexArray, screenIndexBuffer, screenShader);


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}