#ifndef GAME_HPP
#define GAME_HPP

#include "../engine/engine.hpp"

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

        bool checkCollisions(glm::vec3 objectPosition, unsigned int rowID);

        void loop();

        void quit();

        // Inputs callback functions
        static void quitGame();
        static void toggleWireframe();

        static void movePlayerForward();
        static void movePlayerLeft();
        static void movePlayerRight();
        static void movePlayerBackward();
};

#endif