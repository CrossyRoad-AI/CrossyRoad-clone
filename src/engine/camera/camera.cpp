#include <iostream>
#include <stdlib.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "camera.hpp"

Camera::Camera(const glm::vec3 position, const glm::vec3 target) {
    // Set default values
    this->position = position;
    this->target = target;

    this->calcViewMatrix();
}

void Camera::calcViewMatrix() {
    this->viewMatrix = glm::lookAt(this->position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
}