#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera {
    private:
        glm::vec3 position;
        glm::vec3 target;

        glm::mat4 viewMatrix;

    public:
        // Constructor
        Camera(const glm::vec3 position, const glm::vec3 target);

        void calcViewMatrix();

        // Setters and getters
        inline glm::vec3 getPosition() { return this->position; }
        inline glm::vec3 getTarget() { return this->target; }
        inline const float* getViewMatrix() { return &this->viewMatrix[0][0]; }

        inline void move(const glm::vec3 position) { this->position = position; calcViewMatrix(); }
        inline void pointAt(const glm::vec3 target) { this->target = target; calcViewMatrix(); }
};

#endif