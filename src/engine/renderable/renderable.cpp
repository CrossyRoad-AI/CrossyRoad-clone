#include <stdlib.h>

#include <glew.h>
#include <glm.hpp>

#include "renderable.hpp"

#include "../objectLoader/objectLoader.hpp"
#include "../renderer/vertexBuffer/vertexBuffer.hpp"
#include "../renderer/indexBuffer/indexBuffer.hpp"

Renderable::Renderable(std::string modelFilename)
    : modelMatrices(new LinkedList()), updatedData(false)
{
    float* vertexPositions, *vertexColors;
    unsigned int *indices, vertexCount;

    // Load model from filename
    loadModel(modelFilename, &vertexPositions, &vertexColors, &indices, &vertexCount, &this->indicesCount);

    // Generate and bind VAO
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // Creating a buffer of data for vertex positions
    this->vbp = new VertexBuffer(vertexPositions, vertexCount * sizeof(float), GL_STATIC_DRAW);

    // Setup vertex positions structure infos
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // Creating a buffer of data for vertex colors
    this->vbc = new VertexBuffer(vertexColors, vertexCount * sizeof(float), GL_STATIC_DRAW);

    // Setup vertex colors structure infos
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

    // Intance buffer storing all tramformations matrices of instamces
    this->vbi = new VertexBuffer(0, sizeof(glm::mat4) * 0, GL_STREAM_DRAW);

    // Setup instance buffer structure infos, is mat4 so need attrib pointer (wich hold only vec4)
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), 0);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (sizeof(glm::vec4)));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (2 * sizeof(glm::vec4)));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*) (3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    // Creating index buffer
    this->ib = new IndexBuffer(indices, this->indicesCount);

    // Unbund VAO
    glBindVertexArray(0);

    // Free
    free(vertexPositions);
    free(vertexColors);
    free(indices);
}

Renderable::~Renderable() {
    delete this->vbp;
    delete this->vbc;
    delete this->vbi;
    delete this->ib;
}

unsigned int Renderable::addInstance(glm::mat4 modelMatrix) {
    glm::mat4* modelMatrixp = (glm::mat4*) malloc(sizeof(glm::mat4));
    *modelMatrixp = modelMatrix;

    unsigned int index = this->modelMatrices->add(modelMatrixp);
    this->updatedData = true;

    return index;
}

void Renderable::removeInstance(unsigned int instanceID) {
    this->modelMatrices->removeById(instanceID);
    this->updatedData = true;
}

void Renderable::updateInstance(glm::mat4 modelMatrix, unsigned int instanceID) {
    glm::mat4* modelMatrixp = (glm::mat4*) malloc(sizeof(glm::mat4));
    *modelMatrixp = modelMatrix;

    this->modelMatrices->update(instanceID, modelMatrixp);
    this->updatedData = true;
}

void Renderable::updateBuffer() {
    if(this->updatedData) {
        unsigned int count = this->modelMatrices->getCount();
        if(count > 0) {
            glm::mat4* modelMatricesArray = (glm::mat4*) malloc(sizeof(glm::mat4) * count);

            this->modelMatrices->restart();
            for(int i = 0; i < count; i++) modelMatricesArray[i] = *((glm::mat4*) this->modelMatrices->getCurrent());

            glBindVertexArray(this->vao);
            this->vbi->update(modelMatricesArray, sizeof(glm::mat4) * count);
            glBindVertexArray(0);

            free(modelMatricesArray);
        }

        this->updatedData = false;
    }
}

void Renderable::render() {
    unsigned int count = this->modelMatrices->getCount();
    if(count) {
        glBindVertexArray(this->vao);
        glDrawElementsInstanced(GL_TRIANGLES, this->indicesCount, GL_UNSIGNED_INT, 0, count);
        glBindVertexArray(0);
    }
}