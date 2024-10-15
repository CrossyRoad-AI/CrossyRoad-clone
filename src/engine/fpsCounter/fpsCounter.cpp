#include <iostream>
#include <iomanip>
#include <string>
#include <map>

#include <glew.h>
#include <glfw3.h>

#include "fpscounter.hpp"

#include "../textRenderer/textRenderer.hpp"

FPSCounter::FPSCounter(const float updateFrequencyp)
    : updateFrequency(updateFrequencyp), lastUpdateTime(0), frameStart(glfwGetTime()), frameElaspedTime(0)
{
    this->textRenderer = new TextRenderer();
}

void FPSCounter::tick() {
    this->frameStart = glfwGetTime();
}

void FPSCounter::update() {
    float currentTime = glfwGetTime();

    if(currentTime - lastUpdateTime >= this->updateFrequency) {
        this->frameElaspedTime = (currentTime - this->frameStart) * 1000;
        lastUpdateTime = currentTime;
    }


    char text[20]; sprintf(text, "ms / frame: %.2f", this->frameElaspedTime);
    this->textRenderer->renderText(text, 5.0f, 1180.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
}