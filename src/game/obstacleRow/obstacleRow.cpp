#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "obstacleRow.hpp"

#include "../../engine/gameObjects/gameObjects.hpp"
#include "../../engine/utils/linkedList/linkedList.hpp"

ObstacleRow::ObstacleRow(unsigned int id, Renderable* ground, Renderable* obstacle, Renderer* renderer) {
    this->groundObjects = new LinkedList();
    this->obstacles = new LinkedList();

    // Generate ground tiles
    for(int i = 0; i < 9; i++) groundObjects->add(new GameObject(ground, glm::vec3((6 * 6) - i * 6.0f, 0.0f, id * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

    // Generate obstacles
    for(int i = 0; i < rand() % 4; i++) obstacles->add(new GameObject(obstacle, glm::vec3((6 * 6) - (rand() % 9) * 6.0f, 3.0f, id * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
}

ObstacleRow::~ObstacleRow() {
    delete this->groundObjects;
    delete this->obstacles;
}

bool ObstacleRow::checkCollisions(glm::vec3 objectPosition) {
    this->obstacles->restart();
    GameObject* currentObstacle = (GameObject*) this->obstacles->getCurrent();

    while(currentObstacle != NULL) {
        glm::vec3 obstaclePosition = currentObstacle->getPosition();

        if(obstaclePosition.x == objectPosition.x && obstaclePosition.z == objectPosition.z) return true;
        currentObstacle = (GameObject*) this->obstacles->getCurrent();
    }

    return false;
}