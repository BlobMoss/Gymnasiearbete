#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "Engine.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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

    Engine engine;

    float lastElapsedTime = 0.0f;
    float elapsedTime = 0.0f;

    //*

    unsigned int colorTexture;
    glGenTextures(1, &colorTexture);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, referenceWidth * 2, referenceHeight * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    unsigned int normalTexture;
    glGenTextures(1, &normalTexture);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, referenceWidth * 2, referenceHeight * 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    unsigned int depthTexture;
    glGenTextures(1, &depthTexture);

    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, referenceWidth * 2, referenceHeight * 2, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    unsigned int frameBuffer = 0;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, DrawBuffers);



    unsigned int screenTexture;
    glGenTextures(1, &screenTexture);

    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, referenceWidth, referenceHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    unsigned int scaledFrameBuffer = 0;
    glGenFramebuffers(1, &scaledFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, scaledFrameBuffer);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);



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

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexBuffer vb(&screenVertices[0], sizeof(screenVertices));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    screenVertexArray.AddBuffer(vb, layout);


    toonShader.Bind();

    toonShader.SetUniform1i("u_ColorTexture", 1);

    toonShader.SetUniform1i("u_NormalTexture", 2);

    toonShader.SetUniform1i("u_DepthTexture", 3);

    toonShader.SetUniform1i("u_TexWidth", referenceWidth);
    toonShader.SetUniform1i("u_TexHeight", referenceHeight);

    toonShader.SetUniform1f("u_Near", near);
    toonShader.SetUniform1f("u_Far", far);

    glm::mat4 projMat = glm::ortho(0.0f, (float)referenceWidth * 2, 0.0f, (float)referenceHeight * 2, -1.0f, 1.0f);
    toonShader.SetUniformMat4f("u_MVP", projMat);


    screenShader.Bind();

    screenShader.SetUniform1i("u_Texture", 4);

    projMat = glm::ortho(0.0f, (float)referenceWidth, 0.0f, (float)referenceHeight, -1.0f, 1.0f);
    screenShader.SetUniformMat4f("u_MVP", projMat);



    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Updates
        elapsedTime = (float)glfwGetTime();
        float deltaTime = elapsedTime - lastElapsedTime;
        lastElapsedTime = elapsedTime;

        engine.Update(deltaTime);

        //Draws
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glViewport(0, 0, referenceWidth * 2, referenceHeight * 2);
        renderer.Clear();

        engine.Draw(renderer);

        glBindFramebuffer(GL_FRAMEBUFFER, scaledFrameBuffer);
        glViewport(0, 0, referenceWidth * 2, referenceHeight * 2);
        renderer.Clear();

        renderer.Draw(screenVertexArray, screenIndexBuffer, toonShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, referenceWidth * pixelSize, referenceHeight * pixelSize);
        renderer.Clear();

        renderer.Draw(screenVertexArray, screenIndexBuffer, screenShader);

        //Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}