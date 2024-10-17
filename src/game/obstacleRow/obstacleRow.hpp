#ifndef OBSTACLEROW_HPP
#define OBSTACLEROW_HPP

#include "../game.hpp"
#include "../../engine/utils/linkedList/linkedList.hpp"

class ObstacleRow {
    private:
        LinkedList* waterObjects;
        LinkedList* groundObjects;
        LinkedList* obstacles;

        unsigned int rowType;
        unsigned int rowTypeSpecific;

        unsigned int rowIndex;

    public:
        ObstacleRow(const unsigned int rowType, const unsigned int rowIndex, Game* game);
        ~ObstacleRow();

        void update();

        bool checkCollisions(const glm::vec3 objectPosition);
        bool checkGround(glm::vec3 objectPosition);

        // Getters and setters
        inline unsigned int getRowIndex() { return this->rowIndex; }
};

#endif