#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "obstacleRow.hpp"

#include "../../engine/utils/linkedList/linkedList.hpp"

#include "../constants/constants.h"

Renderable* getRandomProp(Renderable** prop, unsigned int count) {
    return prop[rand() % count];
}

ObstacleRow::ObstacleRow(const unsigned int rowTypep, const unsigned int rowIndexp, Game* game)
    : rowIndex(rowIndexp), waterObjects(new LinkedList(this->freeCallback)), groundObjects(new LinkedList(this->freeCallback)), obstacles(new LinkedList(this->freeCallback))
{
    Renderable* ground;
    Renderable* props;

    this->rowType = rowTypep >> 4;
    this->rowTypeSpecific = rowTypep & (0b1111);

    // Select ground tile
    if(this->rowType == ROW_GRASS >> 4) {
        ground = game->grass;

        // Grass always has trees far left and far right
        this->obstacles->add(new GameObject(getRandomProp(game->trees, 3), glm::vec3(-18.0f, 3.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
        this->obstacles->add(new GameObject(getRandomProp(game->trees, 3), glm::vec3(42.0f, 3.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

        // Types of grass rows
        if(this->rowTypeSpecific == ROW_GRASS_FULL_TREE) {
            for(int i = 0; i < 9; i++) this->obstacles->add(new GameObject(getRandomProp(game->trees, 3), glm::vec3(-12.0f + i * 6.0f, 3.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
        } else if(this->rowTypeSpecific == ROW_GRASS_FEW_TREES_RANDOM) {
            for(int i = 0; i < 0; i++) this->obstacles->add(new GameObject(getRandomProp(game->trees, 3), glm::vec3(-12.0f + (rand() % 9) * 6.0f, 3.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
        } else if(this->rowTypeSpecific == ROW_GRASS_ALOT_TREES_RANDOM) {
            for(int i = 0; i < 0; i++) this->obstacles->add(new GameObject(getRandomProp(game->trees, 3), glm::vec3(-12.0f + (rand() % 9) * 6.0f, 3.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
        }

        // rand() % 2

        // Generate ground tiles
        for(int i = 0; i < 11; i++) this->groundObjects->add(new GameObject(ground, glm::vec3((6 * 7) - i * 6.0f, 0.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    } else if(this->rowType == ROW_WATER >> 4) {
        ground = game->water;
        props = game->waterLily;

        if(this->rowTypeSpecific == ROW_WATER_ALOT_WATER_LILLY) {
            for(int i = 0; i < 2 + rand() % 2; i++) this->groundObjects->add(new GameObject(props, glm::vec3(-12.0f + (rand() % 9) * 6.0f, 3.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
        }

        // Generate ground tiles
        for(int i = 0; i < 11; i++) this->waterObjects->add(new GameObject(ground, glm::vec3((6 * 7) - i * 6.0f, 0.0f, this->rowIndex * (-6.0f)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    }
}

ObstacleRow::~ObstacleRow() {
    while(this->waterObjects->getCount()) this->waterObjects->removeByIndex(0);
    while(this->groundObjects->getCount()) this->groundObjects->removeByIndex(0);
    while(this->obstacles->getCount()) this->obstacles->removeByIndex(0);

    delete this->waterObjects;
    delete this->groundObjects;
    delete this->obstacles;
}

void ObstacleRow::update() {
    if(this->rowType == ROW_WATER >> 4) {
        this->groundObjects->restart();
        
        float direction; GameObject* waterLily;
        for(int i = 0; i < this->groundObjects->getCount(); i++) {
            waterLily = (GameObject*) this->groundObjects->getCurrent();

            if(i == 1) direction = -0.4f;
            else direction = 0.4f;
            waterLily->rotate(waterLily->getRotation() + glm::vec3(0.0f, direction, 0.0f));
        }
    }
}

bool ObstacleRow::checkGround(glm::vec3 objectPosition) {
    this->groundObjects->restart();
    
    glm::vec3 groundPosition; GameObject* currentGround;
    for(int i = 0; i < this->groundObjects->getCount(); i++) {
        currentGround = (GameObject*) this->groundObjects->getCurrent();

        groundPosition = currentGround->getPosition();
        if(groundPosition.x == objectPosition.x && groundPosition.z == objectPosition.z) return true;
    }

    return false;
}

bool ObstacleRow::checkCollisions(glm::vec3 objectPosition) {
    this->obstacles->restart();
    
    glm::vec3 obstaclePosition; GameObject* currentObstacle;
    for(int i = 0; i < this->obstacles->getCount(); i++) {
        currentObstacle = (GameObject*) this->obstacles->getCurrent();

        obstaclePosition = currentObstacle->getPosition();
        if(obstaclePosition.x == objectPosition.x && obstaclePosition.z == objectPosition.z) return true;
    }

    return false;
}