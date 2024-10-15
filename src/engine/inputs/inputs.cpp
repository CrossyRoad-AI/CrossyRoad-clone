#include <iostream>
#include <stdlib.h>

#include <glfw3.h>

#include "inputs.hpp"

InputManager::InputManager(GLFWwindow *windowp) {
    this->window = windowp;
    this->eventsToListen = nullptr; this->nbEventsToListen = 0;

    // Capture mouse cursor and disable cursor acceleration
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(glfwRawMouseMotionSupported()) glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // Mouse click callback
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void InputManager::listenKey(unsigned int device, unsigned int key, char mode, CallbackPointer callback) {
    this->eventsToListen = (EventsToListen*) realloc(this->eventsToListen, sizeof(EventsToListen) * (this->nbEventsToListen + 1));
    this->eventsToListen[this->nbEventsToListen] = { .device = device, .key = key, .mode = mode, .lastKeyState = GLFW_RELEASE, .keyCallback = callback};

    this->nbEventsToListen += 1;
}

void InputManager::processInputs() {
    char callCallback, keyState;
    for(int i = 0; i < this->nbEventsToListen; i++) {
        if(this->eventsToListen[i].device == 0) {
            keyState = glfwGetKey(this->window, this->eventsToListen[i].key);

            callCallback = (this->eventsToListen[i].mode == 0 && keyState == GLFW_PRESS);
            callCallback |= (this->eventsToListen[i].mode == 1 && this->eventsToListen[i].lastKeyState == GLFW_PRESS && keyState == GLFW_RELEASE);
            callCallback |= (this->eventsToListen[i].mode == 2 && this->eventsToListen[i].lastKeyState == GLFW_RELEASE && keyState == GLFW_PRESS);
        } else if(this->eventsToListen[i].device == 1) {
            keyState = glfwGetMouseButton(this->window, this->eventsToListen[i].key);

            callCallback = (this->eventsToListen[i].mode == 0 && keyState == GLFW_PRESS);
            callCallback |= (this->eventsToListen[i].mode == 1 && this->eventsToListen[i].lastKeyState == GLFW_PRESS && keyState == GLFW_RELEASE);
            callCallback |= (this->eventsToListen[i].mode == 2 && this->eventsToListen[i].lastKeyState == GLFW_RELEASE && keyState == GLFW_PRESS);
        }

        if(callCallback) (this->eventsToListen[i].keyCallback)();
        this->eventsToListen[i].lastKeyState = keyState;
    }
}