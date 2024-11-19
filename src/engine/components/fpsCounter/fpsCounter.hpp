#ifndef FPSCOUNTER_HPP
#define FPSCOUNTER_HPP

#include "../../ui/textRenderer.hpp"

class FPSCounter {
    private:
        TextRenderer* textRenderer;

        const float updateFrequency;
        float lastUpdateTime;

        float frameStart;
        float frameElaspedTime;

        char text[8];

    public:
        FPSCounter(const float updateFrequency);
        ~FPSCounter();

        void tick();
        void update();
};

#endif