#include <iostream>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "renderer.hpp"
#include "../shaders/shaderLoader.hpp"
#include "../renderable/renderable.hpp"

Renderer::Renderer(GLFWwindow * windowp)
    : window(windowp)
{
    this->renderableCounts = 0; this->renderables = nullptr;

    // Load and compile shader program
    this->shaderProgram = createShaderProgram("./shaders/vertex/vertex.glsl", "./shaders/fragment/fragment.glsl");
    glUseProgram(this->shaderProgram);

    // Get uniforms location
    this->viewLocation = glGetUniformLocation(this->shaderProgram, "view");
    this->projectionLocation = glGetUniformLocation(this->shaderProgram, "projection");

    // Define projection matrix
    float fov = 45.0f;
    this->projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 1200.0f, 0.1f, 200.0f);
}

Renderer::~Renderer() {
    glDeleteProgram(this->shaderProgram);
    free(this->renderables);
}

void Renderer::registerRenderable(Renderable* renderable) {
    this->renderableCounts += 1;
    this->renderables = (Renderable**) realloc(this->renderables, sizeof(Renderable*) * this->renderableCounts);

    this->renderables[this->renderableCounts - 1] = renderable;
}

void Renderer::render() {
    // Select program
    glUseProgram(this->shaderProgram);

    // Set view matrix uniform
    glUniformMatrix4fv(this->viewLocation, 1, GL_FALSE, this->camera->getViewMatrix());

    // Set projection matrix uniform
    glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, &this->projectionMatrix[0][0]);

    // Render objects
    for(int i = 0; i < this->renderableCounts; i++) {
        (this->renderables[i])->updateBuffer();
        (this->renderables[i])->render();
    }
}