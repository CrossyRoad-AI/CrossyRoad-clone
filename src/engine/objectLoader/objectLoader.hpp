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

enum STATES {
    QUERY_MTL = 0x0,
    EXTRACT_DATA,
    EXTRACT_FACES,
};

int loadOBJModel(std::string filename, float** vPositions, float** vColors, float** vNormals, unsigned int** indices, unsigned int* vCount, unsigned int* iCount);
int loadOBJModel(std::string filename, float** vPositions, float** vColors, unsigned int** indices, unsigned int* vCount, unsigned int* iCount);

static int readMTLFile(std::string materialFileName, Color** colors);
static unsigned int updateVertexArray(float** vPositions, float** vColors, float** vNormals, float* tempVPositions, float* tempVNormals, Color color, unsigned int* vertexCount, unsigned int vpIndex, unsigned int vnIndex);

#endif