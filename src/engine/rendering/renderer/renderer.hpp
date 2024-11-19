#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glfw3.h>
#include <glm.hpp>

#include "../../components/camera/camera.hpp"
#include "../../objects/renderable/renderable.hpp"

#include "../../lights/lightRenderer.hpp"

class Renderer {
    private:
        GLFWwindow* window;

        Camera* camera;
        LightRenderer* lightRenderer;

        unsigned int matricesUbo;
        unsigned int viewPosUbo;

        glm::mat4 projectionMatrix;

        unsigned int renderableCounts;
        Renderable** renderables;

    public:
        // Constructor / destructor
        Renderer(GLFWwindow * windowp);
        ~Renderer();

        void registerRenderable(Renderable* renderable);

        void render();

        // Setters and getters
        inline void registerCamera(Camera* camera) { this->camera = camera; }
        inline void registerLightRenderer(LightRenderer* lightRenderer) { this->lightRenderer = lightRenderer; }
};

#endif