#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <glfw3.h>

// Pointer to function void(void), format for key event callback function
typedef void (*CallbackPointer)(void);

typedef struct EventsToListen_ {
    unsigned int device; // 0: key, 1: mouse

    unsigned int key; // GLFW key constant
    char mode; // 0: all, 1: keyup, 2: key down

    char lastKeyState;
    CallbackPointer keyCallback;
} EventsToListen;

class InputManager {
    private:
        GLFWwindow *window;

        unsigned int nbEventsToListen;
        EventsToListen* eventsToListen;

    public:
        // Constructor
        InputManager(GLFWwindow *window);
        ~InputManager();

        void listenKey(unsigned int device, unsigned int key, char mode, CallbackPointer callback);

        void processInputs();
};

#endif