#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "SpriteManager.h"

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

    glClearColor(0.25f, 0.20f, 0.20f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    //glEnable(GL_CULL_FACE);

    Renderer renderer;

    FrameBuffer spriteFrameBuffer;
    spriteFrameBuffer.AddColorTexture(1, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT0);
    spriteFrameBuffer.AddColorTexture(2, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT1);
    spriteFrameBuffer.AddDepthTexture(3, referenceWidth * 2, referenceHeight * 2);

    FrameBuffer screenFrameBuffer;
    screenFrameBuffer.AddColorTexture(4, referenceWidth, referenceHeight, GL_COLOR_ATTACHMENT0);


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
    IndexBuffer screenIndexBuffer(&screenIndices[0], sizeof(screenIndices) / sizeof(*screenIndices));
    Shader toonShader("res/shaders/toon.shader");
    Shader screenShader("res/shaders/screen.shader");

    VertexBuffer vb(&screenVertices[0], sizeof(screenVertices));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    screenVertexArray.AddBuffer(vb, layout);


    toonShader.Bind();

    toonShader.SetUniform1i("u_ColorTexture", 1);
    toonShader.SetUniform1i("u_NormalTexture", 2);
    toonShader.SetUniform1i("u_DepthTexture", 3);

    toonShader.SetUniform1i("u_TexWidth", referenceWidth * 2);
    toonShader.SetUniform1i("u_TexHeight", referenceHeight * 2);

    toonShader.SetUniform1f("u_Near", near);
    toonShader.SetUniform1f("u_Far", far);

    toonShader.SetUniformMat4f("u_MVP", glm::ortho(0.0f, (float)referenceWidth * 2, 0.0f, (float)referenceHeight * 2, -1.0f, 1.0f));


    screenShader.Bind();

    screenShader.SetUniform1i("u_Texture", 4);

    screenShader.SetUniformMat4f("u_MVP", glm::ortho(0.0f, (float)referenceWidth, 0.0f, (float)referenceHeight, -1.0f, 1.0f));


    SpriteManager spriteManager;

    {

    Sprite* gem = new Sprite(new Model(LoadOBJ("res/models/gem.obj"), "res/textures/gem_texture.png", "res/shaders/lighting.shader"));
    gem->SetPosition(glm::vec3(-3.0, 0.0, -1.0));
    spriteManager.AddSprite(gem);

    spriteManager.AddSprite(new Sprite(new Model(LoadOBJ("res/models/teapot.obj"), "res/textures/teapot_texture.png", "res/shaders/lighting.shader")));

    }

    float lastElapsedTime = 0.0f;
    float elapsedTime = 0.0f;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Updating
        elapsedTime = (float)glfwGetTime();
        float deltaTime = elapsedTime - lastElapsedTime;
        lastElapsedTime = elapsedTime;

        spriteManager.Update(deltaTime);

        // Drawing
        spriteFrameBuffer.Bind();
        glViewport(0, 0, referenceWidth * 2, referenceHeight * 2);
        renderer.Clear();

        spriteManager.Draw(renderer);

        screenFrameBuffer.Bind();
        glViewport(0, 0, referenceWidth * 2, referenceHeight * 2);
        renderer.Clear();

        renderer.DrawElements(screenVertexArray, screenIndexBuffer, toonShader);

        screenFrameBuffer.Unbind();
        glViewport(0, 0, referenceWidth * pixelSize, referenceHeight * pixelSize);
        renderer.Clear();

        renderer.DrawElements(screenVertexArray, screenIndexBuffer, screenShader);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}