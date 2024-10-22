#ifndef OBJECT_LOADER_HPP
#define OBJECT_LOADER_HPP

typedef struct Color_ {
    char name[40];

    float ns;
    float ka[3];
    float kd[3];
    float ks[3];
    float ke[3];
    float ni;
    float d;
    int illum;
} Color;

static void updateVertexArray(float** vertexPositions, float** vertexColors, float* tempVertices, Color* colors, unsigned int* correspondances, unsigned int* vertexCount, unsigned int currentColorIndex, unsigned int index);
void loadModel(std::string filename, float** vertexPositions, float** vertexColors, unsigned int** indices, unsigned int* vertexCount, unsigned int* indicesCount);

int loadOBJModel(std::string filename, float** vPositions, float** vColors, float** vNormals, unsigned int** indices, unsigned int* vCount, unsigned int* iCount);

#endif