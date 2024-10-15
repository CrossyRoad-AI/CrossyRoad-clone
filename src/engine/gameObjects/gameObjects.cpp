#include <iostream>
#include <stdlib.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "gameObjects.hpp"

#include "../renderable/renderable.hpp"

GameObject::GameObject(Renderable* renderable, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    c_renderable = renderable;

    c_position = position;
    c_rotation = rotation;
    c_scale = scale;

    calcModelMatrix();
    c_instanceID = c_renderable->addInstance(c_modelMatrix);
}

GameObject::~GameObject() {
    c_renderable->removeInstance(c_instanceID);
}

void GameObject::calcModelMatrix() {
    c_modelMatrix = glm::mat4(1.0f);
    c_modelMatrix = glm::translate(c_modelMatrix, c_position);

    c_modelMatrix = glm::rotate(c_modelMatrix, glm::radians(c_rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    c_modelMatrix = glm::rotate(c_modelMatrix, glm::radians(c_rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    c_modelMatrix = glm::rotate(c_modelMatrix, glm::radians(c_rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

    c_modelMatrix = glm::scale(c_modelMatrix, c_scale);
}

void GameObject::move(float x, float y, float z) {
    c_position[0] = x;
    c_position[1] = y;
    c_position[2] = z;

    calcModelMatrix();
    c_renderable->updateInstance(c_modelMatrix, c_instanceID);
}

void GameObject::rotate(float x, float y, float z) {
    c_rotation[0] = x;
    c_rotation[1] = y;
    c_rotation[2] = z;

    calcModelMatrix();
    c_renderable->updateInstance(c_modelMatrix, c_instanceID);
}