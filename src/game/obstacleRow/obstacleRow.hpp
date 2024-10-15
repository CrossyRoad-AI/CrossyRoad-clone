#ifndef OBSTACLEROW_HPP
#define OBSTACLEROW_HPP

#include "../../engine/renderer/renderer.hpp"
#include "../../engine/utils/linkedList/linkedList.hpp"

class ObstacleRow {
    private:
        LinkedList* groundObjects;
        LinkedList* obstacles;

    public:
        ObstacleRow(unsigned int id, Renderable* renderable, Renderable* obstacle, Renderer* renderer);
        ~ObstacleRow();

        bool checkCollisions(glm::vec3 objectPosition);
};

#endif