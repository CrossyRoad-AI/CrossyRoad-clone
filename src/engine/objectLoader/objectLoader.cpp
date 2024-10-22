#include <iostream>
#include <fstream>
#include <map>
#include <stdlib.h>

#include "objectLoader.hpp"

void updateVertexArray(float** vertexPositions, float** vertexColors, float* tempVertices, Color* colors, unsigned int* correspondances, unsigned int* vertexCount, unsigned int currentColorIndex, unsigned int index) {
    *vertexPositions = (float*) realloc(*vertexPositions, sizeof(float) * (*vertexCount + 3));
    *vertexColors = (float*) realloc(*vertexColors, sizeof(float) * (*vertexCount + 3));

    (*vertexPositions)[*vertexCount] = tempVertices[(index - 1) * 3];
    (*vertexPositions)[*vertexCount + 1] = tempVertices[(index - 1) * 3 + 1];
    (*vertexPositions)[*vertexCount + 2] = tempVertices[(index - 1) * 3 + 2];

    (*vertexColors)[*vertexCount] = colors[currentColorIndex].kd[0];
    (*vertexColors)[*vertexCount + 1] = colors[currentColorIndex].kd[1];
    (*vertexColors)[*vertexCount + 2] = colors[currentColorIndex].kd[2];

    *vertexCount += 3;
    (correspondances)[index] = *vertexCount / 3 - 1;
}

void loadModel(std::string filename, float** vertexPositions, float** vertexColors, unsigned int** indices, unsigned int* vertexCount, unsigned int* indicesCount) {
    // Load obj file
    std::ifstream objFileStream(("models/" + filename + ".obj").c_str());

    // Extract material filename
    std::string materialFileName;
    objFileStream >> materialFileName >> materialFileName;

    // Read material filename
    std::ifstream mtlFileStream(("models/" + materialFileName).c_str());

    // Colors array
    Color* colors = nullptr; unsigned int colorIndex = 0;

    // Extract colors
    std::string mtlWord;
    while(mtlFileStream >> mtlWord) {
        if(mtlWord == "newmtl") {
            colors = (Color*) realloc(colors, sizeof(Color) * (colorIndex + 1));

            mtlFileStream >> colors[colorIndex].name >> mtlWord;
            mtlFileStream >> colors[colorIndex].kd[0] >> colors[colorIndex].kd[1] >> colors[colorIndex].kd[2];

            colorIndex += 1;
        } else break;
    }

    // Initialise arrays and counters
    float* tempVertices = nullptr; unsigned int tempVerticesCount = 0;
    unsigned int *correspondances = nullptr, correspondanceArraySize = 0;

    *vertexPositions = nullptr; *vertexColors = nullptr; *vertexCount = 0;
    *indices = nullptr; *indicesCount = 0;

    // Var to store current state
    std::string objWord;
    std::string textureName;

    unsigned int currentColorIndex = 0;

    unsigned int v1, v2, v3;
    unsigned int maxIndicesPointer;

    // Skip one word
    objFileStream >> objWord;

    // Reading loop
    while(objFileStream >> objWord) {
        if(objFileStream.eof()) break;

        // Extract bunch of vertex
        while(objFileStream >> objWord) {
            if(objWord == "v") {
                tempVertices = (float*) realloc(tempVertices, sizeof(float) * (tempVerticesCount + 3));

                objFileStream >> (tempVertices)[tempVerticesCount] >> (tempVertices)[tempVerticesCount + 1] >> (tempVertices)[tempVerticesCount + 2];
                tempVerticesCount += 3;
            } else break;
        }

        // Skip
        objFileStream >> objWord;

        // Indices reading loop
        while(objFileStream >> objWord) {
            if(objWord == "usemtl") {
                // Clear correspondance array
                currentColorIndex = 0;
                for(int i = 0; i < correspondanceArraySize; i++) correspondances[i] = 0;

                // Read current texture name
                objFileStream >> textureName;

                // Search texture index inside colors array
                do { if(colors[currentColorIndex].name == textureName) break; }
                while(++currentColorIndex);
            } else if(objWord == "f") {
                *indices = (unsigned int*) realloc(*indices, sizeof(unsigned int) * (*indicesCount + 3));
                
                // Read vertex 1, 2 and 3
                objFileStream >> v1 >> v2 >> v3;

                maxIndicesPointer = v1 > v2 ? v1 : (v2 > v3 ? v2 : v3);
                if(maxIndicesPointer >= correspondanceArraySize) {
                    unsigned int lastSize = correspondanceArraySize;
                    correspondances = (unsigned int*) realloc(correspondances, sizeof(unsigned int) * (maxIndicesPointer + 1));

                    // Set to 0 all new elements
                    for(unsigned int i = lastSize; i < maxIndicesPointer + 1; i++) correspondances[i] = 0;
                    correspondanceArraySize = maxIndicesPointer + 1;
                }

                if((correspondances)[v1] == 0) updateVertexArray(vertexPositions, vertexColors, tempVertices, colors, correspondances, vertexCount, currentColorIndex, v1);
                if((correspondances)[v2] == 0) updateVertexArray(vertexPositions, vertexColors, tempVertices, colors, correspondances, vertexCount, currentColorIndex, v2);
                if((correspondances)[v3] == 0) updateVertexArray(vertexPositions, vertexColors, tempVertices, colors, correspondances, vertexCount, currentColorIndex, v3);

                (*indices)[*indicesCount] = (correspondances)[v1];
                (*indices)[*indicesCount + 1] = (correspondances)[v2];
                (*indices)[*indicesCount + 2] = (correspondances)[v3];

                *indicesCount += 3;
            } else break;
        }
    }
    
    // Free data
    free(colors);
    free(tempVertices);
    free(correspondances);
}

// Used to set defaults values to struct
const Color ColorDefaultValues = {"", 0.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.0f, 0.0f, 0};

enum STATES {
    QUERY_MTL = 0x0,
    EXTRACT_DATA,
    EXTRACT_FACES,
};

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
    *vColors = (float*) realloc(*vColors, sizeof(float) * (*vertexCount + 3));
    *vNormals = (float*) realloc(*vNormals, sizeof(float) * (*vertexCount + 3));

    (*vPositions)[*vertexCount] = tempVPositions[vpIndex * 3];
    (*vPositions)[*vertexCount + 1] = tempVPositions[vpIndex * 3 + 1];
    (*vPositions)[*vertexCount + 2] = tempVPositions[vpIndex * 3 + 2];

    (*vColors)[*vertexCount] = color.kd[0];
    (*vColors)[*vertexCount + 1] = color.kd[1];
    (*vColors)[*vertexCount + 2] = color.kd[2];

    (*vNormals)[*vertexCount] = tempVNormals[vnIndex * 3];
    (*vNormals)[*vertexCount + 1] = tempVNormals[vnIndex * 3 + 1];
    (*vNormals)[*vertexCount + 2] = tempVNormals[vnIndex * 3 + 2];

    *vertexCount += 3;
    return *vertexCount / 3 - 1;
}

int loadOBJModel(std::string filename, float** vPositions, float** vColors, float** vNormals, unsigned int** indices, unsigned int* vCount, unsigned int* iCount) {
    // Reset output vars
    *vPositions = nullptr; *vColors = nullptr; *vNormals = nullptr; *vCount = 0;
    *indices = nullptr; *iCount = 0;

    // Load OBJ file
    std::ifstream objFileStream(("models/" + filename + ".obj").c_str());

    // State machine
    unsigned int state = QUERY_MTL;

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
    unsigned int v1, v2, v3, vn1, vn2, vn3, uidV1, uidV2, uidV3;
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
            do { if(colors[colorIndex].name == word) break; }
            while(++colorIndex);

            state = EXTRACT_FACES;
        } else if(word == "f") {
            if(state != EXTRACT_FACES) return -1;

            // Read vertex 1, 2 and 3
            objFileStream >> v1;
            objFileStream.read(trash, 2);
            objFileStream >> vn1;
            
            objFileStream >> v2;
            objFileStream.read(trash, 2);
            objFileStream >> vn2;

            objFileStream >> v3;
            objFileStream.read(trash, 2);
            objFileStream >> vn3;

            // Re-align vp and vn to 0
            v1 --; v2 --; v3 --;
            vn1 --; vn2 --; vn3 --;

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

    return 1;
}

// void loadModel(std::string filename, float** vertexPositions, float** vertexColors, float** vertexNormals, unsigned int** indices, unsigned int* vertexCount, unsigned int* indicesCount) {
//     // // Load obj file
//     // std::ifstream objFileStream(("models/" + filename + ".obj").c_str());

//     // // Extract material filename
//     // std::string materialFileName;
//     // objFileStream >> materialFileName >> materialFileName;

//     // // Read material filename
//     // std::ifstream mtlFileStream(("models/" + materialFileName).c_str());

//     // // Colors array
//     // Color* colors = nullptr; unsigned int colorIndex = 0;

//     // // Extract colors
//     // std::string mtlWord;
//     // while(mtlFileStream >> mtlWord) {
//     //     if(mtlWord == "newmtl") {
//     //         colors = (Color*) realloc(colors, sizeof(Color) * (colorIndex + 1));

//     //         mtlFileStream >> colors[colorIndex].name >> mtlWord;
//     //         mtlFileStream >> colors[colorIndex].red >> colors[colorIndex].green >> colors[colorIndex].blue;

//     //         colorIndex += 1;
//     //     } else break;
//     // }

//     // Initialise arrays and counters
//     // float* tempVerticesPositions = nullptr; unsigned int tempVerticesPositionsCount = 0;
//     // float* tempVerticesNormals = nullptr; unsigned int tempVerticesNormalsCount = 0;
//     // unsigned int *correspondances = nullptr, correspondanceArraySize = 0;

//     // *vertexPositions = nullptr; *vertexColors = nullptr; *vertexNormals = nullptr; *vertexCount = 0;
//     // *indices = nullptr; *indicesCount = 0;

//     // Var to store current state
//     // std::string objWord;
//     // std::string textureName;

//     // unsigned int currentColorIndex = 0;

//     // unsigned int v1, v2, v3;
//     // unsigned int vn1, vn2, vn3;
//     // char trash[3];

//     unsigned int maxIndicesPointer;

//     unsigned int lastSize;
//     unsigned int i;

//     // unsigned int insdexToShift = 0;

//     // Skip one word
//     // objFileStream >> objWord;

//     // Reading loop
//     // while(objFileStream >> objWord) {
//     //     if(objFileStream.eof()) break;

//         // Extract vertex positions and vertex normals
//         // while(objFileStream >> objWord) {
//         //     if(objWord == "v") {
//         //         tempVerticesPositions = (float*) realloc(tempVerticesPositions, sizeof(float) * (tempVerticesPositionsCount + 3));

//         //         objFileStream >> (tempVerticesPositions)[tempVerticesPositionsCount] >> (tempVerticesPositions)[tempVerticesPositionsCount + 1] >> (tempVerticesPositions)[tempVerticesPositionsCount + 2];
//         //         tempVerticesPositionsCount += 3;
//         //     } else if(objWord == "vn") {
//         //         tempVerticesNormals = (float*) realloc(tempVerticesNormals, sizeof(float) * (tempVerticesNormalsCount + 3));

//         //         objFileStream >> (tempVerticesNormals)[tempVerticesNormalsCount] >> (tempVerticesNormals)[tempVerticesNormalsCount + 1] >> (tempVerticesNormals)[tempVerticesNormalsCount + 2];
//         //         tempVerticesNormalsCount += 3;
//         //     } else break;
//         // }

//         // Skip
//         // objFileStream >> objWord;

//         // Indices reading loop
//         // while(objFileStream >> objWord) {
//         //     if(objWord == "usemtl") {
//         //         // Clear correspondance array
//         //         currentColorIndex = 0;
//         //         for(i = 0; i < correspondanceArraySize; i++) correspondances[i] = 0;

//         //         // Read current texture name
//         //         objFileStream >> textureName;

//         //         // Calc shift
//         //         insdexToShift = 0;
//         //         unsigned int currentVal = 0;
//         //         while(currentVal < (tempVerticesNormalsCount / 3) - 1) {
//         //             currentVal = (currentVal << 1) + 1;

//         //             insdexToShift ++;
//         //         }

//         //         // Search texture index inside colors array
//         //         do { if(colors[currentColorIndex].name == textureName) break; }
//         //         while(++currentColorIndex);
//             // } else if(objWord == "f") {
//                 // *indices = (unsigned int*) realloc(*indices, sizeof(unsigned int) * (*indicesCount + 3));
                
//                 // // Read vertex 1, 2 and 3
//                 // objFileStream >> v1;
//                 // objFileStream.read(trash, 2);
//                 // objFileStream >> vn1;
                
//                 // objFileStream >> v2;
//                 // objFileStream.read(trash, 2);
//                 // objFileStream >> vn2;

//                 // objFileStream >> v3;
//                 // objFileStream.read(trash, 2);
//                 // objFileStream >> vn3;

//                 // maxIndicesPointer = ((v1 << insdexToShift) + vn1) > ((v2 << insdexToShift) + vn2) ? ((v1 << insdexToShift) + vn1) : (((v2 << insdexToShift) + vn2) > ((v3 << insdexToShift) + vn3) ? ((v2 << insdexToShift) + vn2) : ((v3 << insdexToShift) + vn3));
//                 // if(maxIndicesPointer >= correspondanceArraySize) {
//                 //     unsigned int lastSize = correspondanceArraySize;
//                 //     correspondances = (unsigned int*) realloc(correspondances, sizeof(unsigned int) * (maxIndicesPointer + 1));

//                 //     // Set to 0 all new elements
//                 //     for(unsigned int i = lastSize; i < maxIndicesPointer + 1; i++) correspondances[i] = 0;
//                 //     correspondanceArraySize = maxIndicesPointer + 1;
//                 // }

//                 // if((correspondances)[((v1 << insdexToShift) + vn1)] == 0) (correspondances)[((v1 << insdexToShift) + vn1)] = updateVertexArray(vertexPositions, vertexColors, vertexNormals, tempVerticesPositions, tempVerticesNormals, colors, correspondances, vertexCount, currentColorIndex, v1, vn1);
//                 // if((correspondances)[((v2 << insdexToShift) + vn2)] == 0) (correspondances)[((v2 << insdexToShift) + vn2)] = updateVertexArray(vertexPositions, vertexColors, vertexNormals, tempVerticesPositions, tempVerticesNormals, colors, correspondances, vertexCount, currentColorIndex, v2, vn2);
//                 // if((correspondances)[((v3 << insdexToShift) + vn3)] == 0) (correspondances)[((v3 << insdexToShift) + vn3)] = updateVertexArray(vertexPositions, vertexColors, vertexNormals, tempVerticesPositions, tempVerticesNormals, colors, correspondances, vertexCount, currentColorIndex, v3, vn3);

//                 (*indices)[*indicesCount] = (correspondances)[((v1 << insdexToShift) + vn1)];
//                 (*indices)[*indicesCount + 1] = (correspondances)[((v2 << insdexToShift) + vn2)];
//                 (*indices)[*indicesCount + 2] = (correspondances)[((v3 << insdexToShift) + vn3)];

//                 *indicesCount += 3;
//             // } else break;
//     //     }
//     // }
    
//     // Free data
//     free(colors);
//     free(tempVerticesPositions);
//     free(tempVerticesNormals);
//     free(correspondances);
// }