#include <stdlib.h>

#include <glew.h>
#include <glm.hpp>

#include <iostream>

#include "renderable.hpp"

#include "../../rendering/shaders/shaderLoader.hpp"
#include "../../components/objectLoader/objectLoader.hpp"
#include "../../rendering/vertexBuffer/vertexBuffer.hpp"
#include "../../rendering/indexBuffer/indexBuffer.hpp"

Renderable::Renderable(std::string modelFilename)
    : modelMatrices(new LinkedList(this->freeCallback)), normalMatrices(new LinkedList(this->freeCallback)), updatedData(false)
{
    float* vertexPositions, *vertexColors, *vertexNormals;
    unsigned int *indices, vertexCount;

    // Load model from filename
    loadOBJModel(modelFilename, &vertexPositions, &vertexColors, &vertexNormals, &indices, &vertexCount, &this->indicesCount);

    // Generate and bind VAO
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // Creating a buffer of data for vertex positions
    this->vbp = new VertexBuffer(vertexPositions, vertexCount * sizeof(float), GL_STATIC_DRAW);

    // Setup vertex positions structure infos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // Creating a buffer of data for vertex colors
    // this->vbc = new VertexBuffer(vertexColors, (vertexCount / 3) * sizeof(RawColor), GL_STATIC_DRAW);
    this->vbc = new VertexBuffer(vertexColors, vertexCount * sizeof(float), GL_STATIC_DRAW);

    // Setup vertex colors structure infos
    // int pointer = 4 * sizeof(float);

    glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RawColor), &pointer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // Creating a buffer of data for vertex normals
    this->vbn = new VertexBuffer(vertexNormals, vertexCount * sizeof(float), GL_STATIC_DRAW);

    // Setup vertex positions structure infos
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // Intance buffer storing all transformations matrices of instamces
    this->vbi = new VertexBuffer(0, sizeof(glm::mat4) * 0, GL_STREAM_DRAW);

    unsigned int offset = 2;

    // Setup instance buffer structure infos, is mat4 so need attrib pointer (wich hold only vec4)
    glEnableVertexAttribArray(offset + 1);
    glEnableVertexAttribArray(offset + 2);
    glEnableVertexAttribArray(offset + 3);
    glEnableVertexAttribArray(offset + 4);

    glVertexAttribPointer(offset + 1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), 0);
    glVertexAttribPointer(offset + 2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (sizeof(glm::vec4)));
    glVertexAttribPointer(offset + 3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (2 * sizeof(glm::vec4)));
    glVertexAttribPointer(offset + 4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(offset + 1, 1);
    glVertexAttribDivisor(offset + 2, 1);
    glVertexAttribDivisor(offset + 3, 1);
    glVertexAttribDivisor(offset + 4, 1);

    offset += 4;

    // Normal matrix
    this->vbnm = new VertexBuffer(0, sizeof(glm::mat3) * 0, GL_STREAM_DRAW);

    // Setup normal matrix buffer structure infos, is mat4 so need attrib pointer (wich hold only vec4)
    glEnableVertexAttribArray(offset + 1);
    glEnableVertexAttribArray(offset + 2);
    glEnableVertexAttribArray(offset + 3);

    glVertexAttribPointer(offset + 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 0);
    glVertexAttribPointer(offset + 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*) (sizeof(glm::vec3)));
    glVertexAttribPointer(offset + 3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), (void*) (2 * sizeof(glm::vec3)));

    glVertexAttribDivisor(offset + 1, 1);
    glVertexAttribDivisor(offset + 2, 1);
    glVertexAttribDivisor(offset + 3, 1);

    // Creating index buffer
    this->ib = new IndexBuffer(indices, this->indicesCount);

    // Unbund VAO
    glBindVertexArray(0);

    // Load and compile shader program
    this->shaderProgram = createShaderProgram("./shaders/vertex/vs_normals.glsl", "./shaders/fragment/fs_normals.glsl");

    // Free
    free(vertexPositions);
    free(vertexColors);
    free(vertexNormals);
    free(indices);
}

Renderable::~Renderable() {
    delete this->vbp;
    delete this->vbc;
    delete this->vbn;

    delete this->vbi;
    delete this->ib;

    glDeleteProgram(this->shaderProgram);
}

unsigned int Renderable::addInstance(glm::mat4 modelMatrix) {
    glm::mat4* modelMatrixp = (glm::mat4*) malloc(sizeof(glm::mat4));
    *modelMatrixp = modelMatrix;

    glm::mat3* normalMatrixp = (glm::mat3*) malloc(sizeof(glm::mat3));
    *normalMatrixp = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

    unsigned int instanceID = this->modelMatrices->add(modelMatrixp);
    this->normalMatrices->add(normalMatrixp);
    this->updatedData = true;

    return instanceID;
}

void Renderable::removeInstance(unsigned int instanceID) {
    this->modelMatrices->removeById(instanceID);
    this->normalMatrices->removeById(instanceID);

    this->updatedData = true;
}

void Renderable::updateInstance(glm::mat4 modelMatrix, unsigned int instanceID) {
    glm::mat4* modelMatrixp = (glm::mat4*) malloc(sizeof(glm::mat4));
    *modelMatrixp = modelMatrix;

    glm::mat3* normalMatrixp = (glm::mat3*) malloc(sizeof(glm::mat3));
    *normalMatrixp = glm::mat3(glm::transpose(glm::inverse(modelMatrix)));

    this->modelMatrices->updateById(instanceID, modelMatrixp);
    this->normalMatrices->updateById(instanceID, normalMatrixp);
    
    this->updatedData = true;
}

void Renderable::updateBuffer() {
    unsigned int count = this->modelMatrices->getCount();
    if(count > 0) {
        glm::mat4* modelMatricesArray = (glm::mat4*) malloc(sizeof(glm::mat4) * count);
        glm::mat3* normalMatricesArray = (glm::mat3*) malloc(sizeof(glm::mat3) * count);

        this->modelMatrices->restart();
        this->normalMatrices->restart();

        for(int i = 0; i < count; i++) {
            modelMatricesArray[i] = *((glm::mat4*) this->modelMatrices->getCurrent());
            normalMatricesArray[i] = *((glm::mat3*) this->normalMatrices->getCurrent());
        }

        glBindVertexArray(this->vao);
        this->vbi->update(modelMatricesArray, sizeof(glm::mat4) * count);
        this->vbnm->update(normalMatricesArray, sizeof(glm::mat3) * count);
        glBindVertexArray(0);

        free(modelMatricesArray);
        free(normalMatricesArray);
    }

    this->updatedData = false;
}

void Renderable::render() {
    if(this->updatedData) this->updateBuffer();

    unsigned int count = this->modelMatrices->getCount();
    if(count) {
        glUseProgram(this->shaderProgram);
        
        glBindVertexArray(this->vao);
        glDrawElementsInstanced(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0, count);
        glBindVertexArray(0);
    }
}