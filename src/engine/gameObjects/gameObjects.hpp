#ifndef GAMEOBJECTS_HPP
#define GAMEOBJECTS_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../renderable/renderable.hpp"

class GameObject {
    private:
        Renderable* c_renderable;

        glm::vec3 c_position;
        glm::vec3 c_rotation;
        glm::vec3 c_scale;

        glm::mat4 c_modelMatrix;

        unsigned int c_instanceID;

    public:
        GameObject(Renderable* renderable, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
        ~GameObject();

        void move(const float x, const float y, const float z);
        void rotate(const float x, const float y, const float z);
        void scale(const float scale);

        void calcModelMatrix();

        // Getters and setters
        inline float getRotateX() { return c_rotation[0]; }
        inline float getRotateY() { return c_rotation[1]; }

        inline glm::vec3 getPosition() { return this->c_position; }
};

#endif