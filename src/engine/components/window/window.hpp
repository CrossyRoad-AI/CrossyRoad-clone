#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <glfw3.h>

GLFWwindow* utils_createWindow(const char* windowName, const unsigned int width, const unsigned int height, const unsigned int frameRateHint, const unsigned int samples, bool enableVSync, bool centered);

void windowResizeCallback(GLFWwindow* window, int width, int height);
void closeWindow(GLFWwindow* window);

#endif