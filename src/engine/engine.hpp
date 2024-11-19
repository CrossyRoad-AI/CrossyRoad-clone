#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "rendering/renderer/renderer.hpp"

#include "objects/renderable/renderable.hpp"
#include "objects/gameObjects/gameObjects.hpp"

#include "components/fpsCounter/fpsCounter.hpp"
#include "components/camera/camera.hpp"
#include "components/inputs/inputs.hpp"

#include "lights/lightRenderer.hpp"

typedef struct WindowParams_ {
    const char* windowName;
    
    const unsigned int width;
    const unsigned int height;
    const unsigned int frameRateHint;
    const unsigned int samples;

    bool enableVSync;
    bool centered;
} WindowParams;

class Engine {
    private:
        Renderer* renderer;
        InputManager* inputsManager;
        FPSCounter* fpscounter;

        GLFWwindow* window;

        bool showFPS;
        bool wireframeMode;
        bool debugMode;

    public:
        // Constructor
        Engine(WindowParams windowParams);

        // Enable / disable settings
        void fpsDisplayMode(const bool showFPS, const float updateFrequency);
        inline void toggleWireframeMode() { this->wireframeMode = !this->wireframeMode; };
        inline void setClearColor(const float r, const float g, const float b, const float a) { glClearColor(r, g, b, a); }

        char loopOnce();

        void checkGlErrors();
        inline void enableDebugMode() { this->debugMode = true; };

        void quit();

        // Getters and setters
        inline GLFWwindow* getWindow() { return this->window; }

        inline void registerRenderer(Renderer* renderer) { this->renderer = renderer; }
        inline void registerInputsManager(InputManager* inputsManager) { this->inputsManager = inputsManager; }
};

#endif