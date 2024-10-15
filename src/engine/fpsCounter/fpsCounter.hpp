#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include "../textRenderer/textRenderer.hpp"

class FPSCounter {
    private:
        TextRenderer* textRenderer;

        const float updateFrequency;
        float lastUpdateTime;

        float frameStart;
        float frameElaspedTime;

    public:
        FPSCounter(const float updateFrequency);

        void tick();
        void update();
};

#endif

// glfwGetTime()