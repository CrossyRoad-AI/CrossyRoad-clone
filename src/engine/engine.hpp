#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "renderer/renderer.hpp"
#include "camera/camera.hpp"
#include "inputs/inputs.hpp"

#include "renderable/renderable.hpp"
#include "gameObjects/gameObjects.hpp"

#include "fpscounter/fpscounter.hpp"

class Engine {
    private:
        Renderer* renderer;
        InputManager* inputsManager;
        FPSCounter* fpscounter;

        GLFWwindow* window;

        bool showFPS;
        bool wireframeMode;

    public:
        // Constructor
        Engine(const char* windowName, const unsigned int width, const unsigned int height);

        // Enable / disable settings
        void fpsDisplayMode(const bool showFPS, const float updateFrequency);
        inline void toggleWireframeMode() { this->wireframeMode = !this->wireframeMode; };

        char loopOnce();
        void checkGlErrors();

        void quit();

        // Getters and setters
        inline GLFWwindow* getWindow() { return this->window; }

        inline void registerRenderer(Renderer* renderer) { this->renderer = renderer; }
        inline void registerInputsManager(InputManager* inputsManager) { this->inputsManager = inputsManager; }
};

#endif