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

    if(showFPS) {
        this->fpscounter = new FPSCounter(updateFrequency);
    } else this->fpscounter = nullptr;
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

// void Engine::prepare() {
//     // Projection
//     float fov = 45.0f;
//     glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 1200.0f, 0.1f, 200.0f);

//     int projectionLocation = glGetUniformLocation(c_shaderProgram, "projection");
//     glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

//     // Camera (view)
//     glm::vec3 cameraPos = glm::vec3(50.0f, 40.0f, 40.0f);
//     glm::vec3 cameraTarget = glm::vec3(-1.0f, -1.0f, -1.0f);

//     glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//     glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); 
//     glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//     glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

//     glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraTarget, cameraUp);

//     int viewLocation = glGetUniformLocation(c_shaderProgram, "view");
//     glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
// }

// void Engine::loop() {
//     // float anglex = 0;
//     // float angley = 0;
//     // float anglez = 0;

//     // int location1 = glGetUniformLocation(c_shaderProgram, "xRotationMatrix");
//     // int location2 = glGetUniformLocation(c_shaderProgram, "yRotationMatrix");
//     // int location3 = glGetUniformLocation(c_shaderProgram, "zRotationMatrix");

//     // const int location = glGetUniformLocation(c_shaderProgram, "u_color");
//     // glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);

//     // float scale[] = {
//     //     0.1, 0, 0,
//     //     0, 0.1, 0,
//     //     0, 0, 0.1,
//     // };
    
//     // glUniformMatrix3fv(glGetUniformLocation(c_shaderProgram, "scaleMatrix"), 1, GL_FALSE, scale);

//     // Projection
//     float fov = 45.0f;
//     glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 1200.0f, 0.1f, 30.0f);

//     int projectionLocation = glGetUniformLocation(c_shaderProgram, "projection");
//     glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

//     // View (camera)
//     // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -1.0f);
//     // glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//     // glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//     // glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

//     glm::mat4 view = glm::mat4(1.0f);
//     // note that we're translating the scene in the reverse direction of where we want to move
//     view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f)); 

//     int viewLocation = glGetUniformLocation(c_shaderProgram, "view");
//     glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

//     // Loop until the user closes the window
//     while(!glfwWindowShouldClose(c_window)) {
//         // Measure speed
//         double currentTime = glfwGetTime();
//         nbFrames++;
//         if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
//             // printf and reset timer
//             printf("%f ms/frame\n", 1000.0 / double(nbFrames));
//             nbFrames = 0;
//             lastTime += 1.0;
//         }

//         c_inputsManager->processInputs(c_window);

//         // anglex = anglex + 0.03;
//         // if(anglex >= 6.28) anglex = 0;

//         // anglez = anglez + 0.01;
//         // if(anglez >= 6.28) anglez = 0;

//         // float xRotationMatrix[] = {
//         //     1.0, 0.0, 0.0,
//         //     0.0, cos(anglex), -sin(anglex),
//         //     0.0, sin(anglex), cos(anglex),
//         // };

//         // float yRotationMatrix[] = {
//         //     cos(angley), 0.0, sin(angley),
//         //     0.0, 1.0, 0.0,
//         //     -sin(angley), 0.0, cos(angley),
//         // };

//         // float zRotationMatrix[] = {
//         //     cos(anglez), -sin(anglez), 0.0,
//         //     sin(anglez), cos(anglez), 0.0,
//         //     0.0, 0.0, 1.0,
//         // };

//         // glUniformMatrix3fv(location1, 1, GL_FALSE, xRotationMatrix);
//         // glUniformMatrix3fv(location2, 1, GL_FALSE, yRotationMatrix);
//         // glUniformMatrix3fv(location3, 1, GL_FALSE, zRotationMatrix);

//         c_renderer->render(c_window);

//         // Check for errors
//         // glCheckErrors();

//         // Poll for and process events
//         glfwPollEvents();
//     }
// }