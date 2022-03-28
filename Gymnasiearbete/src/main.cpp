#include <fstream>
#include <string>
#include <sstream>

#include "Common.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "Renderer.h"
#include "Input.h"

#include "Client.h"

#include "openGL/VertexBuffer.h"
#include "openGL/VertexBufferLayout.h"
#include "openGL/FrameBuffer.h"
#include "stb_image/stb_image.h"

#include "gameplay/SpriteManager.h"
#include "gameplay/BlockGroup.h"
#include "gameplay/Player.h"
#include "ui/UISpriteManager.h"
#include "ui/UIMenuManager.h"

int _tmain(int argc, TCHAR* argv[])
{
    /*
    if (std::is_standard_layout<TYPE>::value)
        std::cout << "yes!" << std::endl;
    else
        std::cout << "no" << std::endl;
    //*/
#pragma region Setup

    srand((unsigned int)time(NULL));

    //

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

    GLFWimage images[1];
    images[0].pixels = stbi_load("res/images/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);

    Input::SetCallbacks(window);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) std::cout << "GLEW is not OK" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Render only faces facing camera
    glEnable(GL_CULL_FACE);

#ifdef NDEBUG
    FreeConsole();
#endif

#pragma endregion 
    // This really should be a seperate class but...
#pragma region Rendering

    FrameBuffer spriteFrameBuffer(referenceWidth * 2, referenceHeight * 2);
    spriteFrameBuffer.AddColorTexture(1, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT0);
    spriteFrameBuffer.AddColorTexture(2, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT1);
    spriteFrameBuffer.AddColorTexture(3, referenceWidth * 2, referenceHeight * 2, GL_COLOR_ATTACHMENT2);
    spriteFrameBuffer.AddDepthTexture(4, referenceWidth * 2, referenceHeight * 2);

    FrameBuffer screenFrameBuffer(referenceWidth * 2, referenceHeight * 2);
    screenFrameBuffer.AddColorTexture(5, referenceWidth, referenceHeight, GL_COLOR_ATTACHMENT0);

    // Set up screen shape:
    // I do not think adding + 0.01 here is the correct solution. Seems to work for now.
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
    VertexBuffer screenVertexBuffer;
    IndexBuffer screenIndexBuffer;
    Shader toonShader("res/shaders/toon.shader");
    Shader screenShader("res/shaders/screen.shader");

    screenVertexBuffer.SetData(&screenVertices[0], sizeof(screenVertices));
    screenIndexBuffer.SetData(&screenIndices[0], sizeof(screenIndices) / sizeof(*screenIndices));

    VertexBufferLayout layout;
    layout.Push<float>(2); // Vertex Position
    layout.Push<float>(2); // Texture Coordinate
    screenVertexArray.SetLayout(layout);


    // Set toon shader uniforms
    toonShader.Bind();

    toonShader.SetUniform1i("u_ColorTexture", 1);
    toonShader.SetUniform1i("u_NormalTexture", 2);
    toonShader.SetUniform1i("u_HighlightTexture", 3);
    toonShader.SetUniform1i("u_DepthTexture", 4);

    toonShader.SetUniform1i("u_TexWidth", referenceWidth * 2);
    toonShader.SetUniform1i("u_TexHeight", referenceHeight * 2);

    toonShader.SetUniform1f("u_Near", nearPlane);
    toonShader.SetUniform1f("u_Far", farPlane);

    toonShader.SetUniformMat4f("u_MVP", glm::ortho(0.0f, (float)referenceWidth * 2, 0.0f, (float)referenceHeight * 2, -1.0f, 1.0f));


    // Set screen shader uniforms
    screenShader.Bind();

    screenShader.SetUniform1i("u_Texture", 5);

    screenShader.SetUniformMat4f("u_MVP", glm::ortho(0.0f, (float)referenceWidth, 0.0f, (float)referenceHeight, -1.0f, 1.0f));

#pragma endregion 

    //

    Client c;
        
    //

    Renderer::gameState = GameState::Menu;
    Renderer::startGame = false;
    Renderer::hostGame = false;
    Renderer::exitGame = false;
    
    //

    UISprite* vignette = new UISprite(new Image("res/images/vignette.png"));
    vignette->m_Image->m_SortingOrder = 0.1f;
    UISpriteManager::AddSprite(vignette);

    UIText* fpsCounter = new UIText();
    fpsCounter->m_Position = glm::uvec2(12, referenceHeight - 12 - 8);
    UISpriteManager::AddSprite(fpsCounter);

    UIMenuManager* menuManager = new UIMenuManager();

    // Keep track of time to calculate time delta
    float lastElapsedTime = 0.0f;
    float elapsedTime = 0.0f;
    // Delay to give user chance to read Fps
    float fpsDelay = 0.0f;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Always Update:

        if (Renderer::exitGame) glfwSetWindowShouldClose(window, true);

        // calculate time delta
        elapsedTime = (float)glfwGetTime();
        float deltaTime = elapsedTime - lastElapsedTime;
        lastElapsedTime = elapsedTime;

        if (Input::KeyDown(KEY_P)) c.PingServer();

        if (Input::KeyDown(KEY_F)) Renderer::ToggleFullscreen();

        // Fps
        if (fpsDelay >= 0.25f)
        {
            fpsCounter->SetText("Fps: " + std::to_string(1.0f / deltaTime).substr(0, 5));
            fpsDelay = 0.0f;
        }
        fpsDelay += deltaTime;

        //

        if (Renderer::gameState == GameState::Menu)
        {
            // Update in menu:

            UISpriteManager::Update(deltaTime);

            if (Renderer::hostGame)
            {
                // Open seperate server application

                STARTUPINFO si;
                PROCESS_INFORMATION pi;

                ZeroMemory(&si, sizeof(si));
                si.cb = sizeof(si);
                ZeroMemory(&pi, sizeof(pi));

                if (!CreateProcess(LPCWSTR(L"NetServer.exe"),
                    argv[1],        // Command line
                    NULL,           // Process handle not inheritable
                    NULL,           // Thread handle not inheritable
                    FALSE,          // Set handle inheritance to FALSE
                    0,              // No creation flags
                    NULL,           // Use parent's environment block
                    NULL,           // Use parent's starting directory 
                    &si,            // Pointer to STARTUPINFO structure
                    &pi)            // Pointer to PROCESS_INFORMATION structure
                    )

                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);

                // Give server time to wake up
                Sleep(100);

                // Find local IP using Google
                asio::io_service netService;
                asio::ip::udp::resolver resolver(netService);
                asio::ip::udp::resolver::query query(asio::ip::udp::v4(), "google.com", "");
                asio::ip::udp::resolver::iterator endpoints = resolver.resolve(query);
                asio::ip::udp::endpoint ep = *endpoints;
                asio::ip::udp::socket socket(netService);
                socket.connect(ep);
                asio::ip::address addr = socket.local_endpoint().address();

                c.Connect(addr.to_string(), 60000);

                // Move game window in front of new server console
                glfwFocusWindow(window);
            }

            if (Renderer::startGame)
            {
                // Get text from input field
                std::string ip = menuManager->GetIPInput();
                bool couldConnect = c.Connect(ip, 60000);
                if (couldConnect)
                {
                    delete menuManager;
                    menuManager = nullptr;

                    Renderer::gameState = GameState::Playing;
                }
                else
                {
                    Renderer::startGame = false;
                    menuManager->m_MenuState = MenuState::ConnectionFailed;
                }
            }
            if (Renderer::hostGame)
            {
                delete menuManager;
                menuManager = nullptr;

                Renderer::gameState = GameState::Playing;
            }

            if (menuManager != nullptr) menuManager->Update(deltaTime);
            
            // Update input arrays 
            Input::Update(deltaTime);
        }
        else if (Renderer::gameState == GameState::Playing)
        {
            // Update while playing:

            if (Input::KeyDown(KEY_ESCAPE)) Renderer::exitGame = true;

            // Update camera
            Camera::Update(deltaTime);

            // Listen for messages
            c.ServerUpdate();

            // Update sprites
            SpriteManager::SaveDescriptions();

            SpriteManager::UpdateLocally(deltaTime);
            UISpriteManager::Update(deltaTime);

            // Send sprite messages
            SpriteManager::UpdateServer();

            // Update input arrays 
            Input::Update(deltaTime);
        }

#pragma region Drawing

        // Bind sprite framebuffer
        spriteFrameBuffer.Bind();
        glClearColor(0.357f, 0.416f, 0.412f, 1.0f);
        Renderer::Clear();

        // Draw sprites on that framebuffer
        SpriteManager::Draw();

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

        UISpriteManager::Draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);

#pragma endregion

        // Poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}