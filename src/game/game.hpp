#ifndef GAME_HPP
#define GAME_HPP

#include "../engine/engine.hpp"
#include "../engine/utils/doubleLinkedList/doubleLinkedList.hpp"

extern unsigned int chickenRowId;

class Game {
    private:

        // Engine objects instances
        Engine* engine;
        InputManager *inputsManager;

        Renderer *renderer;
        Camera* camera;

        unsigned int playerRowIndex;
        unsigned int furthestRowIndex;

    public:
        // Singleton
        Game();

        static Game* getInstance();
        static Game* instance;

        // Objects models
        Renderable* grass;
        Renderable* water;
        Renderable* trees[3];
        Renderable* waterLily;
        Renderable* chicken;

        // Game objects
        DoubleLinkedList* rows;

        GameObject* player;

        void initInputs();

        void loadModels();
        void initRun();

        void playerMove(const glm::vec3 direction, const unsigned int rowId);
        bool checkCollisions(const glm::vec3 objectPosition, const unsigned int rowID);

        void generateNewRows();

        void loop();

        inline void quit() { this->engine->quit(); }

        // Inputs callback functions
        inline static void quitGame() { glfwSetWindowShouldClose(Game::getInstance()->engine->getWindow(), true); }
        inline static void toggleWireframe() { Game::getInstance()->engine->toggleWireframeMode(); }

        inline static void movePlayerForward() { Game* game = Game::getInstance(); game->playerMove(glm::vec3(0.0f, 0.0f, -6.0f), game->playerRowIndex + 1); }
        inline static void movePlayerLeft() { Game* game = Game::getInstance(); game->playerMove(glm::vec3(-6.0f, 0.0f, 0.0f), game->playerRowIndex); }
        inline static void movePlayerRight() { Game* game = Game::getInstance(); game->playerMove(glm::vec3(6.0f, 0.0f, 0.0f), game->playerRowIndex); }
        inline static void movePlayerBackward() { Game* game = Game::getInstance(); game->playerMove(glm::vec3(0.0f, 0.0f, 6.0f), game->playerRowIndex - 1); }
};

#endif