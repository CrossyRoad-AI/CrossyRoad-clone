#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glfw3.h>
#include <glm.hpp>

#include "../camera/camera.hpp"
#include "../renderable/renderable.hpp"

class Renderer {
    private:
        GLFWwindow* window;
        Camera* camera;

        unsigned int ubo;

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
};

#endif