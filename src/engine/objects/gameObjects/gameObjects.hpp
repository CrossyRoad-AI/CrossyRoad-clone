#ifndef GAMEOBJECTS_HPP
#define GAMEOBJECTS_HPP

#include <glm.hpp>

#include "../renderable/renderable.hpp"

class GameObject {
    private:
        Renderable* renderable;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 cscale;

        glm::mat4 modelMatrix;

        unsigned int instanceID;

    public:
        GameObject(Renderable* renderable, const glm::vec3 position, const glm::vec3 rotation, const glm::vec3 scale);
        ~GameObject();

        void move(const glm::vec3 positio);
        void rotate(const glm::vec3 rotation);
        void scale(const float scale);

        void calcModelMatrix();

        // Getters and setters
        inline glm::vec3 getRotation() { return this->rotation; }
        inline glm::vec3 getPosition() { return this->position; }
};

#endif