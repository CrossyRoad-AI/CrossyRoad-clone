#ifndef GAME_HPP
#define GAME_HPP

#include "../engine/engine.hpp"

extern unsigned int chickenRowId;

class Game {
    private:

        // Engine objects instances
        Engine* engine;
        InputManager *inputsManager;

        Renderer *renderer;
        Camera* camera;

        // Objects models
        Renderable* grass;
        Renderable* water;
        Renderable* tree;
        Renderable* waterLily;
        Renderable* chicken;

        GameObject* player;

    public:
        // Singleton
        Game();

        static Game* getInstance();
        static Game* instance;

        void initInputs();

        void loadModels();
        void initRun();

        void playerMove(const glm::vec3 direction, const unsigned int rowId);
        bool checkCollisions(const glm::vec3 objectPosition, const unsigned int rowID);

        void loop();

        inline void quit() { this->engine->quit(); }

        // Inputs callback functions
        inline static void quitGame() { glfwSetWindowShouldClose(Game::getInstance()->engine->getWindow(), true); }
        inline static void toggleWireframe() { Game::getInstance()->engine->toggleWireframeMode(); }

        inline static void movePlayerForward() { Game::getInstance()->playerMove(glm::vec3(0.0f, 0.0f, -6.0f), chickenRowId + 1); }
        inline static void movePlayerLeft() { Game::getInstance()->playerMove(glm::vec3(-6.0f, 0.0f, 0.0f), chickenRowId); }
        inline static void movePlayerRight() { Game::getInstance()->playerMove(glm::vec3(6.0f, 0.0f, 0.0f), chickenRowId); }
        inline static void movePlayerBackward() { Game::getInstance()->playerMove(glm::vec3(0.0f, 0.0f, 6.0f), chickenRowId - 1); }
};

#endif