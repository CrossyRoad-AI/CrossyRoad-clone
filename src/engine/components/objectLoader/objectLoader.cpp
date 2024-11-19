#include <fstream>
#include <map>
#include <stdlib.h>

#include "objectLoader.hpp"

// Used to set defaults values to struct
const Color ColorDefaultValues = {"", 0.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0};

int loadOBJModel(std::string filename, float** vPositions, float** vColors, unsigned int** indices, unsigned int* vCount, unsigned int* iCount) {
    float *vNormals;
    return loadOBJModel(filename, vPositions, vColors, &vNormals, indices, vCount, iCount);
}

int loadOBJModel(std::string filename, float** vPositions, float** vColors, float** vNormals, unsigned int** indices, unsigned int* vCount, unsigned int* iCount) {
    // Reset output vars
    *vPositions = nullptr; *vColors = nullptr; *vNormals = nullptr; *vCount = 0;
    *indices = nullptr; *iCount = 0;

    // Load OBJ file
    std::ifstream objFileStream(("models/" + filename + ".obj").c_str());

    // State machine
    unsigned int state = QUERY_MTL;
    bool useNormals = true;

    // Init general vars
    std::string materialFileName;

    // Array to store colors
    Color* colors = nullptr; unsigned int colorIndex = 0;

    // Temporary vertex positions and normals arrays
    float* tempVerticesPositions = nullptr; unsigned int tempVerticesPositionsCount = 0;
    float* tempVerticesNormals = nullptr; unsigned int tempVerticesNormalsCount = 0;

    // Temporary array for infile vertex index and calculated vertex index
    std::map<int, int> correspondances;

    // Iterating var
    std::string word;

    // Pre-alloc vars to use later in loop
    unsigned int v1, v2, v3, vn1 = 0, vn2 = 0, vn3 = 0, uidV1, uidV2, uidV3;
    char trash[2];

    // Run state machine
    while(objFileStream >> word) {
        if(objFileStream.eof()) break;
        
        if(word == "mtllib") {
            if(state != QUERY_MTL) return -1;

            // Extract filename
            objFileStream >> materialFileName;
            if(!readMTLFile(materialFileName, &colors)) return -2;

            state = EXTRACT_DATA;
        } else if(word == "o") {
            state = EXTRACT_DATA;
        } else if(word == "v") {
            if(state != EXTRACT_DATA) return -1;

            tempVerticesPositions = (float*) realloc(tempVerticesPositions, sizeof(float) * (tempVerticesPositionsCount + 3));
            objFileStream >> (tempVerticesPositions)[tempVerticesPositionsCount ++] >> (tempVerticesPositions)[tempVerticesPositionsCount ++] >> (tempVerticesPositions)[tempVerticesPositionsCount ++];
        } else if(word == "vn") {
            if(state != EXTRACT_DATA) return -1;

            tempVerticesNormals = (float*) realloc(tempVerticesNormals, sizeof(float) * (tempVerticesNormalsCount + 3));
            objFileStream >> (tempVerticesNormals)[tempVerticesNormalsCount ++] >> (tempVerticesNormals)[tempVerticesNormalsCount ++] >> (tempVerticesNormals)[tempVerticesNormalsCount ++];
        } else if(word == "usemtl") {
            // Clear correspondance map
            std::map<int, int>::iterator it;
            for(it = correspondances.begin(); it != correspondances.end(); it ++) correspondances.erase(it->first);

            // Read current texture name
            objFileStream >> word;

            // Search texture index inside colors array
            colorIndex = 0;
            do { if(colors[colorIndex].name == word) break; }
            while(++colorIndex);

            state = EXTRACT_FACES;
        } else if(word == "f") {
            if(state != EXTRACT_FACES) return -1;

            // Read vertex 1, 2 and 3
            objFileStream >> v1;

            objFileStream.read(trash, 1);
            if(useNormals && trash[0] == '/') {
                objFileStream.read(trash, 1);
                objFileStream >> vn1;
            }
            
            objFileStream >> v2;

            if(useNormals) {
                objFileStream.read(trash, 2);
                objFileStream >> vn2;
            }

            objFileStream >> v3;
            
            if(useNormals) {
                objFileStream.read(trash, 2);
                objFileStream >> vn3;
            }

            // Re-align vp and vn to 0
            v1 --; v2 --; v3 --;
            if(useNormals) { vn1 --; vn2 --; vn3 --; }

            uidV1 = (v1 << 16) + vn1;
            uidV2 = (v2 << 16) + vn2;
            uidV3 = (v3 << 16) + vn3;

            // Resize indices array to hold 3 more elements
            *indices = (unsigned int*) realloc(*indices, sizeof(unsigned int) * (*iCount + 3));

            // Check if vertex position / normal combo are in correspondances map
            if(correspondances.find(uidV1) == correspondances.end()) correspondances.insert(std::pair<int, int>(uidV1, updateVertexArray(vPositions, vColors, vNormals, tempVerticesPositions, tempVerticesNormals, colors[colorIndex], vCount, v1, vn1)));
            if(correspondances.find(uidV2) == correspondances.end()) correspondances.insert(std::pair<int, int>(uidV2, updateVertexArray(vPositions, vColors, vNormals, tempVerticesPositions, tempVerticesNormals, colors[colorIndex], vCount, v2, vn2)));
            if(correspondances.find(uidV3) == correspondances.end()) correspondances.insert(std::pair<int, int>(uidV3, updateVertexArray(vPositions, vColors, vNormals, tempVerticesPositions, tempVerticesNormals, colors[colorIndex], vCount, v3, vn3)));
        
            (*indices)[*iCount] = (correspondances)[uidV1];
            (*indices)[*iCount + 1] = (correspondances)[uidV2];
            (*indices)[*iCount + 2] = (correspondances)[uidV3];

            *iCount += 3;
        }
    }

    // printf("vCount: %d\n", *vCount);

    // Free memory
    free(tempVerticesPositions);
    free(tempVerticesNormals);
    free(colors);

    return 1;
}

int readMTLFile(std::string materialFileName, Color** colors) {
    // File reader
    std::ifstream mtlFileStream;

    // Load .mtl file
    mtlFileStream.open(("models/" + materialFileName).c_str());

    // IIterating var
    std::string word;
    int index = -1;

    while(mtlFileStream >> word) {
        if(word == "newmtl") {
            index ++;

            *colors = (Color*) realloc(*colors, sizeof(Color) * (index + 1));
            (*colors)[index] = ColorDefaultValues;

            mtlFileStream >> (*colors)[index].name;
        } else if(word == "Ns") {
            mtlFileStream >> (*colors)[index].ns;
        } else if(word == "Ka") {
            mtlFileStream >> (*colors)[index].ka[0] >> (*colors)[index].ka[1] >> (*colors)[index].ka[2];
        } else if(word == "Kd") {
            mtlFileStream >> (*colors)[index].kd[0] >> (*colors)[index].kd[1] >> (*colors)[index].kd[2];
        } else if(word == "Ks") {
            mtlFileStream >> (*colors)[index].ks[0] >> (*colors)[index].ks[1] >> (*colors)[index].ks[2];
        } else if(word == "Ke") {
            mtlFileStream >> (*colors)[index].ke[0] >> (*colors)[index].ke[1] >> (*colors)[index].ke[2];
        } else if(word == "Ni") {
            mtlFileStream >> (*colors)[index].ni;
        } else if(word == "d") {
            mtlFileStream >> (*colors)[index].d;
        } else if(word == "illum") {
            mtlFileStream >> (*colors)[index].illum;
        } else return -1;
    }

    return 1;
}

unsigned int updateVertexArray(float** vPositions, float** vColors, float** vNormals, float* tempVPositions, float* tempVNormals, Color color, unsigned int* vertexCount, unsigned int vpIndex, unsigned int vnIndex) {
    *vPositions = (float*) realloc(*vPositions, sizeof(float) * (*vertexCount + 3));
    // *vColors = (float*) realloc(*vColors, sizeof(RawColor) * (*vertexCount / 3 + 1));
    *vColors = (float*) realloc(*vColors, sizeof(float) * (*vertexCount + 3));
    *vNormals = (float*) realloc(*vNormals, sizeof(float) * (*vertexCount + 3));

    (*vPositions)[*vertexCount] = tempVPositions[vpIndex * 3];
    (*vPositions)[*vertexCount + 1] = tempVPositions[vpIndex * 3 + 1];
    (*vPositions)[*vertexCount + 2] = tempVPositions[vpIndex * 3 + 2];

    // int baseIncrement = (sizeof(RawColor) / sizeof(float)) * (*vertexCount / 3);
    // (*vColors)[baseIncrement] = color.ns;
    // (*vColors)[baseIncrement + 1] = color.ka[0];
    // (*vColors)[baseIncrement + 2] = color.ka[1];
    // (*vColors)[baseIncrement + 3] = color.ka[2];
    // (*vColors)[baseIncrement + 4] = color.kd[0];
    // (*vColors)[baseIncrement + 5] = color.kd[1];
    // (*vColors)[baseIncrement + 6] = color.kd[2];
    // (*vColors)[baseIncrement + 7] = color.ks[0];
    // (*vColors)[baseIncrement + 8] = color.ks[1];
    // (*vColors)[baseIncrement + 9] = color.ks[2];
    // (*vColors)[baseIncrement + 10] = color.ke[0];
    // (*vColors)[baseIncrement + 11] = color.ke[1];
    // (*vColors)[baseIncrement + 12] = color.ke[2];
    // (*vColors)[baseIncrement + 13] = color.ni;
    // (*vColors)[baseIncrement + 14] = color.d;
    // (*vColors)[baseIncrement + 15] = color.illum;

    (*vColors)[*vertexCount] = color.kd[0];
    (*vColors)[*vertexCount + 1] = color.kd[1];
    (*vColors)[*vertexCount + 2] = color.kd[2];

    (*vNormals)[*vertexCount] = tempVNormals[vnIndex * 3];
    (*vNormals)[*vertexCount + 1] = tempVNormals[vnIndex * 3 + 1];
    (*vNormals)[*vertexCount + 2] = tempVNormals[vnIndex * 3 + 2];

    *vertexCount += 3;
    return *vertexCount / 3 - 1;
}