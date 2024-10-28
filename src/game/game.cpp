#include <iostream>

#include "../engine/engine.hpp"
#include "../engine/utils/doubleLinkedList/doubleLinkedList.hpp"
#include "../engine/gameObjects/gameObjects.hpp"

#include "constants/constants.h"

#include "game.hpp"
#include "obstacleRow/obstacleRow.hpp"

extern char* memoryBuffer;
bool sendDataRequest = true;
bool gameRunning = true;
bool shouldRestartGame = false;

void sendData() {
    Game* game = Game::getInstance();

    // Clear memory buffer
    for(int i = 0; i < 5000; i++) {
        memoryBuffer[i] = 0;
    }

    memcpy(&memoryBuffer[1], &game->playerScore, 4); // Current score as int
    memoryBuffer[5] = (gameRunning ? 1 : 0); // Player state, 1 en vie, 0 mort --> edit

    // Player position
    glm::vec3 playerPos = game->player->getPosition();
    memcpy(&memoryBuffer[6], &playerPos.x, 4);
    memcpy(&memoryBuffer[10], &playerPos.z, 4);

    unsigned int grassCount = 0; float* grassPositions = nullptr;
    unsigned int waterCount = 0; float* waterPositions = nullptr;
    unsigned int treesCount = 0; float* treesPositions = nullptr;
    unsigned int waterLilyCount = 0; float* waterLilyPositions = nullptr;

    game->rows->restart(); ObstacleRow* row;
    for(int i = 0; i < game->rows->getCount(); i++) {
        row = (ObstacleRow*) game->rows->getElementByIndex(i);

        if(row->getType() == ROW_GRASS >> 4) {
            // Store grass tiles
            grassPositions = (float*) realloc(grassPositions, sizeof(float) * ((grassCount + 11) * 2));

            row->groundObjects->restart();
            for(int j = 0; j < 11; j++) {
                glm::vec3 grassPosition = ((GameObject*) row->groundObjects->getCurrent())->getPosition();
                grassPositions[grassCount * 2 + j * 2] = grassPosition.x;
                grassPositions[grassCount * 2 + j * 2 + 1] = grassPosition.z;
            }

            grassCount += 11;

            // Store trees
            treesPositions = (float*) realloc(treesPositions, sizeof(float) * ((treesCount + row->obstacles->getCount()) * 2));

            row->obstacles->restart();
            for(int j = 0; j < row->obstacles->getCount(); j++) {
                glm::vec3 treePosition = ((GameObject*) row->obstacles->getCurrent())->getPosition();
                treesPositions[treesCount * 2 + j * 2] = treePosition.x;
                treesPositions[treesCount * 2 + j * 2 + 1] = treePosition.z;
            }

            treesCount += row->obstacles->getCount();
        }

        if(row->getType() == ROW_WATER >> 4) {
            // Store water tiles positions
            waterPositions = (float*) realloc(waterPositions, sizeof(float) * ((waterCount + 11) * 2));

            row->waterObjects->restart();
            for(int j = 0; j < 11; j++) {
                glm::vec3 waterPosition = ((GameObject*) row->waterObjects->getCurrent())->getPosition();
                waterPositions[waterCount * 2 + j * 2] = waterPosition.x;
                waterPositions[waterCount * 2 + j * 2 + 1] = waterPosition.z;
            }

            waterCount += 11;

            // Store water lily positions
            waterLilyPositions = (float*) realloc(waterLilyPositions, sizeof(float) * ((waterLilyCount + row->groundObjects->getCount()) * 2));

            row->groundObjects->restart();
            for(int j = 0; j < row->groundObjects->getCount(); j++) {
                glm::vec3 waterLilYPosition = ((GameObject*) row->groundObjects->getCurrent())->getPosition();
                waterLilyPositions[waterLilyCount * 2 + j * 2] = waterLilYPosition.x;
                waterLilyPositions[waterLilyCount * 2 + j * 2 + 1] = waterLilYPosition.z;
            }

            waterLilyCount += row->groundObjects->getCount();
        }
    }

    unsigned int offset = 14;

    // Store nb grass tiles and grass tiles positions
    memcpy(&memoryBuffer[offset], &grassCount, 4);
    if(grassCount) memcpy(&memoryBuffer[offset + 4], grassPositions, grassCount * 2 * 4);
    offset = offset + 4 + grassCount * 2 * 4;

    // Store nb water tiles and grass tiles positions
    memcpy(&memoryBuffer[offset], &treesCount, 4);
    if(treesCount) memcpy(&memoryBuffer[offset + 4], treesPositions, treesCount * 2 * 4);
    offset = offset + 4 + treesCount * 2 * 4;

    // Store nb water tiles and grass tiles positions
    memcpy(&memoryBuffer[offset], &waterCount, 4);
    if(waterCount) memcpy(&memoryBuffer[offset + 4], waterPositions, waterCount * 2 * 4);
    offset = offset + 4 + waterCount * 2 * 4;

    // Store nb water tiles and grass tiles positions
    memcpy(&memoryBuffer[offset], &waterLilyCount, 4);
    if(waterLilyCount) memcpy(&memoryBuffer[offset + 4], waterLilyPositions, waterLilyCount * 2 * 4);
    offset = offset + 4 + waterLilyCount * 2 * 4;

    // // Int de fin
    unsigned int max = 4294967295; // All ones
    memcpy(&memoryBuffer[offset], &max, 4);

    // Data is ready
    memoryBuffer[0] = 1;

    // Reset flag
    sendDataRequest = false;
}

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
    this->inputsManager->listenKey(0, GLFW_KEY_ENTER, 2, Game::restartGame);
    this->inputsManager->listenKey(0, GLFW_KEY_SPACE, 2, Game::toggleWireframe);

    this->inputsManager->listenKey(0, GLFW_KEY_W, 2, Game::movePlayerForward);
    this->inputsManager->listenKey(0, GLFW_KEY_A, 2, Game::movePlayerLeft);
    this->inputsManager->listenKey(0, GLFW_KEY_D, 2, Game::movePlayerRight);
    this->inputsManager->listenKey(0, GLFW_KEY_S, 2, Game::movePlayerBackward);
}

void Game::loadModels() {
    this->trees[0] = new Renderable("plants/trees/small/tree_small_normals", true);
    this->renderer->registerRenderable(this->trees[0]);

    this->grass = new Renderable("ground/grass/grass_normals", true);
    this->renderer->registerRenderable(this->grass);

    this->water = new Renderable("ground/water/water_normals", true);
    this->renderer->registerRenderable(this->water);

    this->trees[1] = new Renderable("plants/trees/mid/tree_mid_normals", true);
    this->renderer->registerRenderable(this->trees[1]);

    this->trees[2] = new Renderable("plants/trees/big/tree_big_normals", true);
    this->renderer->registerRenderable(this->trees[2]);

    this->waterLily = new Renderable("plants/water-lily/water-lily_normals", true);
    this->renderer->registerRenderable(this->waterLily);
    
    this->chicken = new Renderable("players/chicken/chicken_normals", true);
    this->renderer->registerRenderable(this->chicken);
}

void Game::initRun() {
    this->playerRowIndex = 5;
    this->playerScore = 0;
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
    sendDataRequest = true;

    glm::vec3 playerPos = this->player->getPosition();
    glm::vec3 playerRot = this->player->getRotation();
    playerPos += direction;

    if(!((ObstacleRow*) this->rows->getElementById(rowIndex))->checkGround(playerPos)) {
        gameRunning = false;
        shouldRestartGame = false;
    } else if(!((ObstacleRow*) this->rows->getElementById(rowIndex))->checkCollisions(playerPos)) {
        if(direction.x < 0) playerRot.y = -90;
        if(direction.x > 0) playerRot.y = 90;
        if(direction.z > 0) playerRot.y = 0;
        if(direction.z < 0) playerRot.y = 180;
        
        this->player->move(playerPos);
        this->player->rotate(playerRot);
        
        this->camera->pointAt(playerPos);

        this->camera->move(this->camera->getPosition() + direction);
        this->playerRowIndex = rowIndex;
        this->playerScore = this->playerRowIndex - 5;
    } else {
        // Collide
    }
}

void Game::loop() {
    do {
        // Update all rows
        this->rows->restart(); ObstacleRow* row;
        for(int i = 0; i < this->rows->getCount(); i++) {
            row = (ObstacleRow*) this->rows->getElementByIndex(i);

            // Remove rows too far behind
            if(row->getRowIndex() + ROWS_BEHIND_PLAYER < this->playerRowIndex) delete (ObstacleRow*) this->rows->removeByIndex(0);
            else row->update();
        }

        // Generate new rows
        while(this->playerRowIndex + FURTHEST_GENERATED_ROW > this->furthestRowIndex) this->generateNewRows();

        // Write data to shared memory
        if(sendDataRequest) sendData();
    } while(!this->engine->loopOnce() && gameRunning);

    sendData();

    // Wait for restart
    do {
        if(shouldRestartGame) this->restart();
    } while(!this->engine->loopOnce());
}

void Game::generateNewRows() {
    unsigned int rowType = 0; // = rand() % 2;
    switch(rowType) {
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

void Game::restart() {
    // Clear current game data
    while(this->rows->getCount()) delete (ObstacleRow*) this->rows->removeByIndex(0);
    
    delete this->renderer;

    this->renderer = new Renderer(this->engine->getWindow());
    this->renderer->registerCamera(this->camera);
    this->engine->registerRenderer(this->renderer);

    // Refill data
    this->loadModels();
    this->initRun();

    sendDataRequest = true;
    shouldRestartGame = false;
    gameRunning = true;

    this->loop();
}

// double xpos = 0, ypos = 0;
// double xposlast = 0, yposlast = 0;
// void moveCamera() {
//     glm::vec3 position = camera->getPosition();
//     glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), (float) (yposlast - ypos) / 200.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//     glm::mat3 rotationMatrixFull = glm::rotate(rotationMatrixX, (float) (xposlast - xpos) / 200.0f, glm::vec3(0.0f, 1.0f, 0.0f));

//     glm::vec3 positionp = rotationMatrixFull * position;
//     camera->move(glm::vec3(positionp[0], positionp[1], positionp[2]));
// }