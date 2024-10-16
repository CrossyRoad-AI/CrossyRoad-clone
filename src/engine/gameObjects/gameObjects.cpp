#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "gameObjects.hpp"

#include "../renderable/renderable.hpp"

GameObject::GameObject(Renderable* renderablep, const glm::vec3 positionp, const glm::vec3 rotationp, const glm::vec3 scalep)
    : renderable(renderablep), position(positionp), rotation(rotationp), cscale(scalep)
{
    calcModelMatrix();
    this->instanceID = this->renderable->addInstance(this->modelMatrix);
}

GameObject::~GameObject() {
    printf("deleted \n");
    this->renderable->removeInstance(this->instanceID);
}

void GameObject::calcModelMatrix() {
    this->modelMatrix = glm::mat4(1.0f);
    this->modelMatrix = glm::translate(this->modelMatrix, this->position);

    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation[0]), glm::vec3(1.0f, 0.0f, 0.0f));
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation[1]), glm::vec3(0.0f, 1.0f, 0.0f));
    this->modelMatrix = glm::rotate(this->modelMatrix, glm::radians(this->rotation[2]), glm::vec3(0.0f, 0.0f, 1.0f));

    this->modelMatrix = glm::scale(this->modelMatrix, this->cscale);
}

void GameObject::move(const glm::vec3 position) {
    this->position = position;

    calcModelMatrix();
    this->renderable->updateInstance(this->modelMatrix, this->instanceID);
}

void GameObject::rotate(const glm::vec3 rotation) {
    this->rotation = rotation;

    calcModelMatrix();
    this->renderable->updateInstance(this->modelMatrix, this->instanceID);
}

void GameObject::scale(const float scale) {
    this->cscale = cscale;

    calcModelMatrix();
    this->renderable->updateInstance(this->modelMatrix, this->instanceID);
}