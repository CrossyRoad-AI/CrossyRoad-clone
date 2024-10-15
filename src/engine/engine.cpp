#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include "engine.hpp"
#include "window/window.hpp"

#include "fpscounter/fpscounter.hpp"

Engine::Engine(const char* windowName, const unsigned int width, const unsigned int height)
    : renderer(nullptr), inputsManager(nullptr), 
    showFPS(false), wireframeMode(false)
{
    // Initialize GLFW
    if(glfwInit() == GL_FALSE) {
        std::cout << "GLFW init error" << std::endl;
        exit(1);
    }

    // Create window
    this->window = createWindow(windowName, width, height);

    // Initialize glew
    if(glewInit() != GLEW_OK) {
        std::cout << "Glew init error" << std::endl;
        exit(2);
    }

    // Print OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Enable antialiasing
    glEnable(GL_MULTISAMPLE);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Setup color blending mode
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup the screen clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Engine::fpsDisplayMode(const bool showFPSp, const float updateFrequency) {
    this->showFPS = showFPSp;

    if(showFPS) this->fpscounter = new FPSCounter(updateFrequency);
    else this->fpscounter = nullptr;
}

char Engine::loopOnce() {
    // Check for quit event
    if(glfwWindowShouldClose(this->window)) return 1;
    
    // FPS counter
    if(this->showFPS) this->fpscounter->tick();

    // Trigger action based on registered input actions
    if(this->inputsManager) this->inputsManager->processInputs();

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable wireframe mode
    if(this->wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render all object registered for this scene
    if(this->renderer) this->renderer->render();

    // Restore to filled rendering mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // FPS counter
    if(this->showFPS) this->fpscounter->update();

    // Swap front and back buffers
    glfwSwapBuffers(this->window);

    // Check for errors
    this->checkGlErrors();

    // Poll for and process events
    glfwPollEvents();

    return 0;
}

void Engine::quit() {
    // In window file, close GLFW window
    if(this->window) closeWindow(this->window);

    glfwTerminate();
}

void Engine::checkGlErrors() {
    while(GLenum error = glGetError()) {
        std::cout << "OpenGL error: " << error << std::endl;
        exit(7);
    }
}