#include <stdlib.h>

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "renderer.hpp"
#include "../renderable/renderable.hpp"

Renderer::Renderer(GLFWwindow * windowp)
    : window(windowp), renderableCounts(0), renderables(nullptr)
{
    // Define projection matrix
    float fov = 45.0f;
    this->projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 1200.0f, 0.1f, 200.0f);

    glGenBuffers(1, &this->ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &this->projectionMatrix[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &this->ubo);
    free(this->renderables);
}

void Renderer::registerRenderable(Renderable* renderable) {
    this->renderableCounts += 1;
    this->renderables = (Renderable**) realloc(this->renderables, sizeof(Renderable*) * this->renderableCounts);

    this->renderables[this->renderableCounts - 1] = renderable;
}

void Renderer::render() {
    // Set view matrix
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), this->camera->getViewMatrix());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Render objects
    for(int i = 0; i < this->renderableCounts; i++) {
        this->renderables[i]->render();
    }
}