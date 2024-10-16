#include <iostream>

#include "../engine/engine.hpp"
#include "../engine/utils/doubleLinkedList/doubleLinkedList.hpp"

#include "game.hpp"
#include "obstacleRow/obstacleRow.hpp"

Game* Game::instance = nullptr;

// Global const
const int  maxDistanceRow = 15;

unsigned int chickenRowId = 0;
unsigned int furthestRowId = 0;

// Obstacle list
DoubleLinkedList *obstaclesList = new DoubleLinkedList();

Game* Game::getInstance() {
    if(Game::instance == nullptr) Game::instance = new Game();
    return Game::instance;
}

Game::Game() {
    this->engine = new Engine("Crossy road", 800, 1200);
    this->engine->fpsDisplayMode(true, 0.2f);

    // this->camera = new Camera(glm::vec3(45.0f, 90.0f, 70.0f), glm::vec3(10.0f, 3.0f, - (5.0f * 3.0f)));
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
    // Load objects models
    this->grass = new Renderable("ground/grass/grass");
    this->renderer->registerRenderable(this->grass);

    this->water = new Renderable("ground/water/water");
    this->renderer->registerRenderable(this->water);

    this->tree = new Renderable("plants/tree/CR_Tree_L1");
    this->renderer->registerRenderable(this->tree);

    this->waterLily = new Renderable("plants/water-lily/water-lily");
    this->renderer->registerRenderable(this->waterLily);

    this->chicken = new Renderable("players/chicken/CR_Chicken");
    this->renderer->registerRenderable(this->chicken);
}

void Game::initRun() {
    // Generate instances of objects
    for(furthestRowId = 0; furthestRowId < maxDistanceRow; furthestRowId++) {
        obstaclesList->add(new ObstacleRow(furthestRowId, this->grass, this->tree, this->renderer));
    }

    this->player = new GameObject(this->chicken, glm::vec3(12.0f, 3.90f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(0.385f, 0.385f, 0.385f));

    glm::vec3 playerPos = this->player->getPosition();
    this->camera->pointAt(playerPos);
    this->camera->move(glm::vec3(playerPos.x + 15, playerPos.y + 60, playerPos.z + 40));
}

void Game::playerMove(const glm::vec3 direction, const unsigned int rowId) {
    glm::vec3 playerPos = this->player->getPosition();
    playerPos += direction;

    if(!this->checkCollisions(playerPos, rowId)) {
        this->player->move(playerPos);
        this->camera->pointAt(playerPos);

        this->camera->move(this->camera->getPosition() + direction);
        chickenRowId = rowId;
    } else {
        // Collide
    }
}

bool Game::checkCollisions(const glm::vec3 objectPosition, const unsigned int rowID) {
    ObstacleRow* currentRow = (ObstacleRow*) obstaclesList->getElementById(rowID);
    return currentRow->checkCollisions(objectPosition);
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

        // Add more lines
        while(chickenRowId + maxDistanceRow > furthestRowId) {
            obstaclesList->add(new ObstacleRow(furthestRowId, this->grass, this->tree, this->renderer));
            furthestRowId ++;
        }

        unsigned int count = obstaclesList->getCount();
        while(count > 24) {
            delete (ObstacleRow*) obstaclesList->removeByIndex(0);
            count --;
        }
    } while(!this->engine->loopOnce());
}