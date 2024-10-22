#include <iostream>

#include "../engine/engine.hpp"
#include "../engine/utils/doubleLinkedList/doubleLinkedList.hpp"
#include "../engine/gameObjects/gameObjects.hpp"

#include "constants/constants.h"

#include "game.hpp"
#include "obstacleRow/obstacleRow.hpp"

extern char* memoryBuffer;
bool sendDataRequest = false;
bool restartGame = false;

// void sendData() {
//     Game* game = Game::getInstance();

//     // Date is not ready
//     memoryBuffer[0] = 0;

//     // Store game state data
//     memcpy(&memoryBuffer[1], &game->playerRowIndex, 4); // Current score as int
//     memoryBuffer[5] = (restartGame ? 0 : 1); // Player state, 1 en vie, 0 mort --> edit

//     // Player position
//     glm::vec3 playerPos =  game->player->getPosition();
//     memcpy(&memoryBuffer[6], &playerPos.x, 4);
//     memcpy(&memoryBuffer[10], &playerPos.y, 4);

//     unsigned int grassCount = 0; float* grassPositions = nullptr;
//     unsigned int waterCount = 0; float* waterPositions = nullptr;
//     unsigned int treesCount = 0; float* treesPositions = nullptr;
//     unsigned int waterLilyCount = 0; float* waterLilyPositions = nullptr;

//     game->rows->restart(); ObstacleRow* row;
//     std::cout << "Rows count " << game->rows->getCount() << std::endl;
//     for(int i = 0; i < game->rows->getCount(); i++) {
//         row = (ObstacleRow*) game->rows->getElementByIndex(i);

//         if(row->getType() == ROW_GRASS >> 4) {
//             std::cout << "Row is grass" << std::endl;

//             // Store grass tiles
//             grassPositions = (float*) realloc(grassPositions, sizeof(float) * ((grassCount + 11) * 2));


//             row->groundObjects->restart();
//             for(int j = 0; j < 11; j++) {
//                 glm::vec3 grassPosition = ((GameObject*) row->groundObjects->getCurrent())->getPosition();
//                 grassPositions[grassCount * 2 + j * 2] = grassPosition.x;
//                 grassPositions[grassCount * 2 + j * 2 + 1] = grassPosition.y;
//             }

//             grassCount += 11;

//             printf("test \n");

//             // Store trees
//             treesPositions = (float*) realloc(treesPositions, sizeof(float) * ((treesCount + row->obstacles->getCount()) * 2));


//             row->obstacles->restart();
//             for(int j = 0; j < row->obstacles->getCount(); j++) {
//                 glm::vec3 treePosition = ((GameObject*) row->obstacles->getCurrent())->getPosition();
//                 treesPositions[treesCount * 2 + j * 2] = treePosition.x;
//                 treesPositions[treesCount * 2 + j * 2 + 1] = treePosition.y;
//             }

//             treesCount += row->obstacles->getCount();

//             printf("test \n");
//         }

//         if(row->getType() == ROW_WATER >> 4) {
//             std::cout << "Row is water" << std::endl;
            
//             // Store water tiles positions
//             waterPositions = (float*) realloc(waterPositions, sizeof(float) * ((waterCount + 11) * 2));

//             row->waterObjects->restart();
//             for(int j = 0; j < 11; j++) {
//                 glm::vec3 waterPosition = ((GameObject*) row->waterObjects->getCurrent())->getPosition();
//                 waterPositions[waterCount * 2 + j * 2] = waterPosition.x;
//                 waterPositions[waterCount * 2 + j * 2 + 1] = waterPosition.y;
//             }

//             waterCount += 11;

//             printf("test \n");

//             // Store water lily positions
//             waterLilyPositions = (float*) realloc(waterLilyPositions, sizeof(float) * ((waterLilyCount + row->groundObjects->getCount()) * 2));

//             row->groundObjects->restart();
//             for(int j = 0; j < row->groundObjects->getCount(); j++) {
//                 glm::vec3 waterLilYPosition = ((GameObject*) row->groundObjects->getCurrent())->getPosition();
//                 waterLilyPositions[waterLilyCount * 2 + j * 2] = waterLilYPosition.x;
//                 waterLilyPositions[waterLilyCount * 2 + j * 2 + 1] = waterLilYPosition.y;
//             }

//             waterLilyCount += row->groundObjects->getCount();

//             printf("test \n");
//         }
//     }

//     unsigned int offset = 14;

//     printf("write mem \n");

//     // Store nb grass tiles and grass tiles positions
//     memcpy(&memoryBuffer[offset], &grassCount, 4);
//     printf("write mem \n");

//     std::cout << offset << std::endl;
//     std::cout << grassCount << std::endl;
//     std::cout << grassPositions[0] << std::endl;

//     if(grassCount > 0) memcpy(&memoryBuffer[offset + 4], &grassPositions, grassCount * 2 * 4);

//     printf("write mem \n");

//     std::cout << offset << std::endl;

//     offset = offset + 4 + grassCount * 2 * 4;

//     printf("write mem \n");

//     std::cout << offset << std::endl;

//     // Store nb water tiles and grass tiles positions
//     memcpy(&memoryBuffer[offset], &treesCount, 4);
//     if(treesCount) memcpy(&memoryBuffer[offset + 4], &treesPositions, treesCount * 2 * 4);

//     offset = offset + 4 + treesCount * 2 * 4;

//     printf("write mem \n");

//     std::cout << offset << std::endl;

//     // Store nb water tiles and grass tiles positions
//     memcpy(&memoryBuffer[offset], &waterCount, 4);
//     if(waterCount) memcpy(&memoryBuffer[offset + 4], &waterPositions, waterCount * 2 * 4);

//     offset = offset + 4 + waterCount * 2 * 4;

//     printf("write mem \n");

//     std::cout << offset << std::endl;

//     // Store nb water tiles and grass tiles positions
//     memcpy(&memoryBuffer[offset], &waterLilyCount, 4);
//     if(waterLilyCount) memcpy(&memoryBuffer[offset + 4], &waterLilyPositions, waterLilyCount * 2 * 4);

//     printf("write mem \n");

//     // // Int de fin
//     unsigned int max = 4294967294; // All ones
//     memcpy(&memoryBuffer[offset + 4], &max, 4);

//     printf("write mem \n");

//     // Data is ready
//     memoryBuffer[0] = 1;

//     printf("write mem \n");

//     // Reset flag
//     sendDataRequest = false;
// }

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
    this->trees[0] = new Renderable("plants/trees/small/tree_small_normals", true);
    this->renderer->registerRenderable(this->trees[0]);

    this->grass = new Renderable("ground/grass/grass", false);
    this->renderer->registerRenderable(this->grass);

    this->water = new Renderable("ground/water/water", false);
    this->renderer->registerRenderable(this->water);

    this->trees[1] = new Renderable("plants/trees/mid/tree_mid", false);
    this->renderer->registerRenderable(this->trees[1]);

    this->trees[2] = new Renderable("plants/trees/big/tree_big", false);
    this->renderer->registerRenderable(this->trees[2]);

    this->waterLily = new Renderable("plants/water-lily/water-lily", false);
    this->renderer->registerRenderable(this->waterLily);
    
    this->chicken = new Renderable("players/chicken/CR_Chicken", false);
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
    sendDataRequest = true;

    glm::vec3 playerPos = this->player->getPosition();
    playerPos += direction;

    if(!((ObstacleRow*) this->rows->getElementById(rowIndex))->checkGround(playerPos)) {
        restartGame = true;
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

        // Write data to shared memory
        // if(sendDataRequest) sendData();
    } while(!this->engine->loopOnce() && !restartGame);

    if(restartGame) {
        restartGame = false;
        this->restart();
    }
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