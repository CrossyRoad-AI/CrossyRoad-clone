#include <stdlib.h>
#include <iostream>

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "lightRenderer.hpp"

LightRenderer::LightRenderer() {
    // Create ambient light UBO
    glGenBuffers(1, &this->ambientLightUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, this->ambientLightUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, 3, this->ambientLightUBO);
}

LightRenderer::~LightRenderer() {

}

void LightRenderer::setAmbientLight(glm::vec3 color, float intensity) {
    this->ambientLightColor = color;
    this->ambientLightIntensity = intensity;

    glm::vec3 ambientLight = this->ambientLightColor; // * this->ambientLightIntensity;
    std::cout << "Ambient light: " << ambientLight.x << ", " << ambientLight.y << ", " << ambientLight.z << std::endl;

    // Set ambient color
    glBindBuffer(GL_UNIFORM_BUFFER, this->ambientLightUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &(ambientLight[0]));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightRenderer::render() {
    
}