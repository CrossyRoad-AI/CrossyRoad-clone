#include <iostream>

#include <glfw3.h>

#include "window.hpp"

GLFWwindow* createWindow(const char* windowName, const unsigned int width, const unsigned int height) {
    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = (GLFWwindow*) malloc(sizeof(GLFWwindow*));

    // Hard select opengl version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Set window settings
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    glfwWindowHint(GLFW_SAMPLES, 8);

    window = glfwCreateWindow(width, height, windowName, NULL, NULL);

    if (!window) {
        glfwTerminate();
        
        std::cout << "Window creation error" << std::endl;
        exit(1);
    }

    // Center window
    int xpos, ypos, winowWidth, windowHeight;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(monitor, &xpos, &ypos, &winowWidth, &windowHeight);
    glfwSetWindowPos(window, (int) ((winowWidth - width) / 2), (int) ((windowHeight - height) / 2));

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);

    // Enable v-sync (minimum 1 display refresh before new frame rendering)
    glfwSwapInterval(1);

    // Show the window
    glfwRestoreWindow(window);

    // Listen window resize event
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);

    return window;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void closeWindow(GLFWwindow* window) {
    glfwDestroyWindow(window);
}