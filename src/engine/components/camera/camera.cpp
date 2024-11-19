#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "camera.hpp"

Camera::Camera(const glm::vec3 positionp, const glm::vec3 targetp)
    : position(positionp), target(targetp)
{
    this->calcViewMatrix();
}

void Camera::calcViewMatrix() {
    this->viewMatrix = glm::lookAt(this->position, this->target, glm::vec3(0.0f, 1.0f, 0.0f));
}