#ifndef OBJECT_LOADER_HPP
#define OBJECT_LOADER_HPP

typedef struct Color_ {
    char name[40];
    float red;
    float green;
    float blue;
} Color;

static void updateVertexArray(float** vertexPositions, float** vertexColors, float* tempVertices, Color* colors, unsigned int* correspondances, unsigned int* vertexCount, unsigned int currentColorIndex, unsigned int index);
void loadModel(std::string filename, float** vertexPositions, float** vertexColors, unsigned int** indices, unsigned int* vertexCount, unsigned int* indicesCount);

#endif