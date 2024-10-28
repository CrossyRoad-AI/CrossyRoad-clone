#include <glm.hpp>

#include "fpscounter.hpp"

#include "../textRenderer/textRenderer.hpp"

FPSCounter::FPSCounter(const float updateFrequencyp)
    : updateFrequency(updateFrequencyp), lastUpdateTime(0), frameStart(glfwGetTime()), frameElaspedTime(0)
{
    this->textRenderer = new TextRenderer(16);
}

FPSCounter::~FPSCounter() {
    delete this->textRenderer;
}

void FPSCounter::tick() {
    this->frameStart = glfwGetTime();
}

void FPSCounter::update() {
    float currentTime = glfwGetTime();

    if(currentTime - lastUpdateTime >= this->updateFrequency) {
        this->frameElaspedTime = (currentTime - this->frameStart) * 1000;
        lastUpdateTime = currentTime;

        this->text[8]; sprintf(this->text, "%.2f ms", this->frameElaspedTime);
    }

    this->textRenderer->renderText(this->text, 5.0f, 1180.0f, 1.0f, glm::vec3(0.8f, 0.8f, 0.8f));
}