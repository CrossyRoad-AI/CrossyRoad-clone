#include <stdlib.h>
#include <map>

#include <glfw3.h>

#include "inputs.hpp"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
std::map<unsigned int, int> keyPressed;

InputManager::InputManager(GLFWwindow *windowp)
    : window(windowp), eventsToListen(nullptr), nbEventsToListen(0)
{
    // Capture mouse cursor and disable cursor acceleration
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if(glfwRawMouseMotionSupported()) glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // Mouse click callback
    // glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Keybord callback
    glfwSetKeyCallback(window, keyCallback);
}

InputManager::~InputManager() {
    free(this->eventsToListen);
}

void InputManager::listenKey(char device, unsigned int key, char mode, CallbackPointer callback) {
    this->eventsToListen = (EventsToListen*) realloc(this->eventsToListen, sizeof(EventsToListen) * (this->nbEventsToListen + 1));
    this->eventsToListen[this->nbEventsToListen] = { .device = device, .key = key, .mode = mode, .lastKeyState = GLFW_RELEASE, .keyCallback = callback};

    keyPressed.insert(std::pair<unsigned int, int>(key, GLFW_RELEASE));

    this->nbEventsToListen += 1;
}

void InputManager::processInputs() {
    char callCallback, keyState;
    for(int i = 0; i < this->nbEventsToListen; i++) {
        if(this->eventsToListen[i].device == 0) {
            // keyState = glfwGetKey(this->window, this->eventsToListen[i].key);

            keyState = keyPressed[this->eventsToListen[i].key];
            keyPressed[this->eventsToListen[i].key] = GLFW_RELEASE;
        } else if(this->eventsToListen[i].device == 1) keyState = glfwGetMouseButton(this->window, this->eventsToListen[i].key);

        callCallback = (this->eventsToListen[i].mode == 0 && keyState == GLFW_PRESS);
        callCallback |= (this->eventsToListen[i].mode == 1 && this->eventsToListen[i].lastKeyState == GLFW_PRESS && keyState == GLFW_RELEASE);
        callCallback |= (this->eventsToListen[i].mode == 2 && this->eventsToListen[i].lastKeyState == GLFW_RELEASE && keyState == GLFW_PRESS);

        if(callCallback) (this->eventsToListen[i].keyCallback)();
        this->eventsToListen[i].lastKeyState = keyState;
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    keyPressed[key] = action == GLFW_PRESS ? GLFW_PRESS : keyPressed[key];
}