#include <iostream>
#include <string>
#include <map>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H 

#include "textRenderer.hpp"

#include "../rendering/shaders/shaderLoader.hpp"

TextRenderer::TextRenderer(unsigned int fontSize) {
    FT_Library ft;
    if(FT_Init_FreeType(&ft)) {
        std::cout << "Could not init FreeType Library" << std::endl;
        exit(1);
    }

    FT_Face face;
    if(FT_New_Face(ft, "fonts/Roboto.ttf", 0, &face)) {
        std::cout << "Failed to load font" << std::endl;  
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize); // Set font size
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

    for(unsigned char c = 0; c < 128; c++) {
        // Load character glyph 
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "Failed to load Glyph" << std::endl;
            exit(1);
        }

        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        this->characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Load specific shader program for text rendering
    this->shaderProgram = createShaderProgram("./shaders/vertex/text_vertex.glsl", "./shaders/fragment/text_fragment.glsl");

    // Extract uniforms locations
    glUseProgram(this->shaderProgram);
    this->projectionLocation = glGetUniformLocation(this->shaderProgram, "projection");
    this->textColorLocation = glGetUniformLocation(this->shaderProgram, "textColor");
    glUseProgram(0);

    // Generate VAO for texture rendering
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    // Generate VBO
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    // Unbind all buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Save projection matrix
    this->projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 1200.0f);
}

TextRenderer::~TextRenderer() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);

    glDeleteProgram(this->shaderProgram);
}

void TextRenderer::renderText(const std::string text, float x, const float y, const float scale, const glm::vec3 color) {
    // Activate corresponding render state	
    glUseProgram(this->shaderProgram);

    // Set unifiorms values
    glUniformMatrix4fv(this->projectionLocation, 1, GL_FALSE, &this->projectionMatrix[0][0]);
    glUniform3f(this->textColorLocation, color.x, color.y, color.z);

    // Active vao and texture buffers
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->vao);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = this->characters[*c];

        // Calc positions x / y and width / height
        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h, 0.0f, 0.0f },            
            { xpos,     ypos,     0.0f, 1.0f },
            { xpos + w, ypos,     1.0f, 1.0f },

            { xpos,     ypos + h, 0.0f, 0.0f },
            { xpos + w, ypos,     1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    // Unbind buffers
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}