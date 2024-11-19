#ifndef LIGHT_RENDERER_HPP
#define LIGHT_RENDERER_HPP

#include <glm.hpp>

class LightRenderer {
    private:
        // Ambiant light
        glm::vec3 ambientLightColor;
        float ambientLightIntensity;

        unsigned int ambientLightUBO;

    public:
        LightRenderer();
        ~LightRenderer();

        void setAmbientLight(glm::vec3 color, float intensity);

        void render();

        // void addLight(Light* light);
        // void removeLight(Light* light);
        // void clearLights();
};

#endif