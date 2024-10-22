#include <iostream>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "renderer.hpp"
#include "../renderable/renderable.hpp"

Renderer::Renderer(GLFWwindow * windowp)
    : window(windowp), renderableCounts(0), renderables(nullptr)
{
    // Load and compile shader program
    // this->shaderProgram = createShaderProgram("./shaders/vertex/vertex.glsl", "./shaders/fragment/fragment.glsl");
    // glUseProgram(this->shaderProgram);

    // Get uniforms location
    // this->projectionLocation = glGetUniformLocation(this->shaderProgram, "projection");
    // this->viewLocation = glGetUniformLocation(this->shaderProgram, "view");

    // Define projection matrix
    float fov = 45.0f;
    this->projectionMatrix = glm::perspective(glm::radians(fov), 800.0f / 1200.0f, 0.1f, 200.0f);

    glGenBuffers(1, &this->uboExampleBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, this->uboExampleBlock);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // glUniformBlockBinding(shaderProgram, 0, 0); // --> with shader program building
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uboExampleBlock);

    glBindBuffer(GL_UNIFORM_BUFFER, this->uboExampleBlock);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &this->projectionMatrix[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

Renderer::~Renderer() {
    // glDeleteProgram(this->shaderProgram);
    free(this->renderables);
}

void Renderer::registerRenderable(Renderable* renderable) {
    this->renderableCounts += 1;
    this->renderables = (Renderable**) realloc(this->renderables, sizeof(Renderable*) * this->renderableCounts);

    this->renderables[this->renderableCounts - 1] = renderable;
}

void Renderer::render() {
    // Select program
    // glUseProgram(this->shaderProgram);

    // Set view matrix uniform
    // glUniformMatrix4fv(this->viewLocation, 1, GL_FALSE, this->camera->getViewMatrix());
    glBindBuffer(GL_UNIFORM_BUFFER, this->uboExampleBlock);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), this->camera->getViewMatrix());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Set projection matrix uniform
    // glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, &this->projectionMatrix[0][0]);

    // Render objects
    for(int i = 0; i < this->renderableCounts; i++) {
        this->renderables[i]->updateBuffer(); // Place somewhere else
        this->renderables[i]->render();
    }
}