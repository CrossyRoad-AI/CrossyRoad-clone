#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "objectLoader.hpp"

void updateVertexArray(float** vertexPositions, float** vertexColors, float* tempVertices, Color* colors, unsigned int* correspondances, unsigned int* vertexCount, unsigned int currentColorIndex, unsigned int index) {
    *vertexPositions = (float*) realloc(*vertexPositions, sizeof(float) * (*vertexCount + 3));
    *vertexColors = (float*) realloc(*vertexColors, sizeof(float) * (*vertexCount + 3));

    (*vertexPositions)[*vertexCount] = tempVertices[(index - 1) * 3];
    (*vertexPositions)[*vertexCount + 1] = tempVertices[(index - 1) * 3 + 1];
    (*vertexPositions)[*vertexCount + 2] = tempVertices[(index - 1) * 3 + 2];

    (*vertexColors)[*vertexCount] = colors[currentColorIndex].red;
    (*vertexColors)[*vertexCount + 1] = colors[currentColorIndex].green;
    (*vertexColors)[*vertexCount + 2] = colors[currentColorIndex].blue;

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
            mtlFileStream >> colors[colorIndex].red >> colors[colorIndex].green >> colors[colorIndex].blue;

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