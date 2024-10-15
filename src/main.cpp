#include <iostream>
#include <time.h>
#include <stdlib.h>

#include "game/game.hpp"

// double xpos = 0, ypos = 0;
// double xposlast = 0, yposlast = 0;
// void moveCamera() {
//     glm::vec3 position = camera->getPosition();
//     glm::mat4 rotationMatrixX = glm::rotate(glm::mat4(1.0f), (float) (yposlast - ypos) / 200.0f, glm::vec3(1.0f, 0.0f, 0.0f));
//     glm::mat3 rotationMatrixFull = glm::rotate(rotationMatrixX, (float) (xposlast - xpos) / 200.0f, glm::vec3(0.0f, 1.0f, 0.0f));

//     glm::vec3 positionp = rotationMatrixFull * position;
//     camera->move(glm::vec3(positionp[0], positionp[1], positionp[2]));
// }

int main() {
    srand(time(NULL));
    
    Game* game = Game::getInstance();
    game->initInputs();

    game->loadModels();

    game->initRun();
    game->loop();

    game->quit();

    return 0;
}