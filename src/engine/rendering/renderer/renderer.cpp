#include <stdlib.h>
#include <iostream>

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "renderer.hpp"
#include "../../objects/renderable/renderable.hpp"

Renderer::Renderer(GLFWwindow * windowp)
    : window(windowp), renderableCounts(0), renderables(nullptr)
{
    // Define projection matrix
    float fov = 45.0f;
    this->projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 1200.0f, 0.1f, 200.0f);

    // Create Matrices UBO
    glGenBuffers(1, &this->matricesUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->matricesUbo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->matricesUbo);

    // Set view matrix
    glBindBuffer(GL_UNIFORM_BUFFER, this->matricesUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &this->projectionMatrix[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Create viewPos UBO
    glGenBuffers(1, &this->viewPosUbo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->viewPosUbo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->viewPosUbo);
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &this->matricesUbo);
    glDeleteBuffers(1, &this->viewPosUbo);

    free(this->renderables);
}

void Renderer::registerRenderable(Renderable* renderable) {
    this->renderableCounts += 1;
    this->renderables = (Renderable**) realloc(this->renderables, sizeof(Renderable*) * this->renderableCounts);

    this->renderables[this->renderableCounts - 1] = renderable;
}

void Renderer::render() {
    // Set view matrix
    glBindBuffer(GL_UNIFORM_BUFFER, this->matricesUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), this->camera->getViewMatrix());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Set view position
    glBindBuffer(GL_UNIFORM_BUFFER, this->viewPosUbo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &this->camera->getPosition()[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Load light data
    this->lightRenderer->render();

    // Render objects
    for(int i = 0; i < this->renderableCounts; i++) {
        this->renderables[i]->render();
    }
}