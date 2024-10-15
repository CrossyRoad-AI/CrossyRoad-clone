#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glfw3.h>

GLFWwindow* createWindow(const char* windowName, const unsigned int width, const unsigned int height);
void windowResizeCallback(GLFWwindow* window, int width, int height);
void closeWindow(GLFWwindow* window);

#endif