#ifndef TEXTRENDERER_HPP
#define TEXTRENDERER_HPP

#include <string>

#include <glfw3.h>
#include <glm.hpp>

struct Character {
    unsigned int textureID; // ID handle of the glyph texture
    glm::ivec2 size; // Size of glyph
    glm::ivec2 bearing; // Offset from baseline to left/top of glyph
    long int advance; // Offset to advance to next glyph
};

class TextRenderer {
    private:
        GLFWwindow* window;

        unsigned int shaderProgram;
        unsigned int vao, vbo;

        unsigned int textColorLocation;
        unsigned int projectionLocation;

        glm::mat4 projectionMatrix;

    public:
        // Constructor / destructor
        TextRenderer(unsigned int fontSize);
        ~TextRenderer();

        void renderText(const std::string text, float x, const float y, const float scale, const glm::vec3 color);
};

#endif