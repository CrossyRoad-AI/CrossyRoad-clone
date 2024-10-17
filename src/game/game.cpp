#include <iostream>

#include "../engine/engine.hpp"
#include "../engine/utils/doubleLinkedList/doubleLinkedList.hpp"

#include "constants/constants.h"

#include "game.hpp"
#include "obstacleRow/obstacleRow.hpp"

// Game class singletin
Game* Game::instance = nullptr;

Game* Game::getInstance() {
    if(Game::instance == nullptr) Game::instance = new Game();
    return Game::instance;
}

Game::Game()
    : rows(new DoubleLinkedList())
{
    this->engine = new Engine("Crossy road", 800, 1200);
    this->engine->fpsDisplayMode(true, 0.2f);

    this->camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    this->renderer = new Renderer(this->engine->getWindow());
    this->renderer->registerCamera(this->camera);

    this->inputsManager = new InputManager(this->engine->getWindow());

    this->engine->registerRenderer(this->renderer);
    this->engine->registerInputsManager(this->inputsManager);
}

void Game::initInputs() {
    this->inputsManager->listenKey(0, GLFW_KEY_ESCAPE, 0, Game::quitGame);
    this->inputsManager->listenKey(0, GLFW_KEY_SPACE, 2, Game::toggleWireframe);

    this->inputsManager->listenKey(0, GLFW_KEY_W, 2, Game::movePlayerForward);
    this->inputsManager->listenKey(0, GLFW_KEY_A, 2, Game::movePlayerLeft);
    this->inputsManager->listenKey(0, GLFW_KEY_D, 2, Game::movePlayerRight);
    this->inputsManager->listenKey(0, GLFW_KEY_S, 2, Game::movePlayerBackward);
}

void Game::loadModels() {
    this->grass = new Renderable("ground/grass/grass");
    this->renderer->registerRenderable(this->grass);

    this->water = new Renderable("ground/water/water");
    this->renderer->registerRenderable(this->water);

    this->trees[0] = new Renderable("plants/trees/small/tree_small");
    this->renderer->registerRenderable(this->trees[0]);

    this->trees[1] = new Renderable("plants/trees/mid/tree_mid");
    this->renderer->registerRenderable(this->trees[1]);

    this->trees[2] = new Renderable("plants/trees/big/tree_big");
    this->renderer->registerRenderable(this->trees[2]);

    this->waterLily = new Renderable("plants/water-lily/water-lily");
    this->renderer->registerRenderable(this->waterLily);

    this->chicken = new Renderable("players/chicken/CR_Chicken");
    this->renderer->registerRenderable(this->chicken);
}

void Game::initRun() {
    this->playerRowIndex = 5;
    this->furthestRowIndex = 6;

    this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_FULL_TREE, 0, this));
    this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_EMPTY, 1, this));
    this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_EMPTY, 2, this));
    this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_EMPTY, 3, this));
    this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_EMPTY, 4, this));
    this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_EMPTY, 5, this));

    // Player
    this->player = new GameObject(this->chicken, glm::vec3(12.0f, 3.90f, playerRowIndex * (-6.0f)), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(0.385f, 0.385f, 0.385f));

    // Set camera to point to player
    glm::vec3 playerPos = this->player->getPosition();
    this->camera->pointAt(playerPos);
    this->camera->move(glm::vec3(playerPos.x + 15, playerPos.y + 60, playerPos.z + 40));
}

void Game::playerMove(const glm::vec3 direction, const unsigned int rowIndex) {
    glm::vec3 playerPos = this->player->getPosition();
    playerPos += direction;

    if(!((ObstacleRow*) this->rows->getElementById(rowIndex))->checkGround(playerPos)) {
        printf("player drowned \n");
    } else if(!((ObstacleRow*) this->rows->getElementById(rowIndex))->checkCollisions(playerPos)) {
        this->player->move(playerPos);
        this->camera->pointAt(playerPos);

        this->camera->move(this->camera->getPosition() + direction);
        this->playerRowIndex = rowIndex;
    } else {
        // Collide
    }
}

void Game::loop() {
    do {
        // float y = this->gameObjectWaterLily1->getRotateY() + 0.4;
        // this->gameObjectWaterLily1->rotate(0, y, 0);

        // y = gameObjectWaterLily2->getRotateY() - 0.3;
        // gameObjectWaterLily2->rotate(0, y, 0);

        // xposlast = xpos;
        // yposlast = ypos;
        // glfwGetCursorPos(engine->getWindow(), &xpos, &ypos);

        // glm::vec3 position = camera->getPosition();
        // camera->move(glm::vec3(position[0], position[1], position[2] - 0.05f));

        // glm::vec3 target = camera->getTarget();
        // camera->pointAt(glm::vec3(target[0], target[1], target[2] - 0.05f));

        // Update all rows
        this->rows->restart(); ObstacleRow* row;
        for(int i = 0; i < this->rows->getCount(); i++) {
            row = (ObstacleRow*) this->rows->getElementByIndex(i);

            // Remove too far behind rows
            if(row->getRowIndex() + ROWS_BEHIND_PLAYER < this->playerRowIndex) delete (ObstacleRow*) this->rows->removeByIndex(0);
            else row->update();
        }

        // Generate new rows
        while(this->playerRowIndex + FURTHEST_GENERATED_ROW > this->furthestRowIndex) this->generateNewRows();
    } while(!this->engine->loopOnce());
}

void Game::generateNewRows() {
    switch(rand() % 2) {
        unsigned int nbGrassRows;
        unsigned int nbWaterRows;

        case 0:
            nbWaterRows = 1;
            nbGrassRows = 1 + rand() % 2;

            for(int i = 0; i < nbWaterRows; i++) {
                this->rows->add(new ObstacleRow(ROW_WATER, this->furthestRowIndex, this));
                this->furthestRowIndex++;
            }

            for(int i = 0; i < nbGrassRows; i++) {
                this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_FEW_TREES_RANDOM, this->furthestRowIndex, this));
                this->furthestRowIndex++;
            }
        break;

        case 1:
            nbGrassRows = 1 + rand() % 2;

            for(int i = 0; i < nbGrassRows; i++) {
                this->rows->add(new ObstacleRow(ROW_GRASS | ROW_GRASS_ALOT_TREES_RANDOM, this->furthestRowIndex, this));
                this->furthestRowIndex++;
            }
        break;
    }
}