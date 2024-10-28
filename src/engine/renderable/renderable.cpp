#include <stdlib.h>

#include <glew.h>
#include <glm.hpp>

#include "renderable.hpp"

#include "../shaders/shaderLoader.hpp"
#include "../objectLoader/objectLoader.hpp"
#include "../renderer/vertexBuffer/vertexBuffer.hpp"
#include "../renderer/indexBuffer/indexBuffer.hpp"

Renderable::Renderable(std::string modelFilename, bool useNormalsp)
    : modelMatrices(new LinkedList()), updatedData(false), useNormals(useNormalsp)
{
    float* vertexPositions, *vertexColors, *vertexNormals;
    unsigned int *indices, vertexCount;

    // Load model from filename
    if(this->useNormals) loadOBJModel(modelFilename, &vertexPositions, &vertexColors, &vertexNormals, &indices, &vertexCount, &this->indicesCount);
    else loadOBJModel(modelFilename, &vertexPositions, &vertexColors, &indices, &vertexCount, &this->indicesCount);

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

    unsigned int offset;
    if(this->useNormals) {
        // Creating a buffer of data for vertex normals
        this->vbn = new VertexBuffer(vertexNormals, vertexCount * sizeof(float), GL_STATIC_DRAW);

        // Setup vertex positions structure infos
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

        offset = 2;
    } else offset = 1;

    // Intance buffer storing all transformations matrices of instamces
    this->vbi = new VertexBuffer(0, sizeof(glm::mat4) * 0, GL_STREAM_DRAW);

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

    // Creating index buffer
    this->ib = new IndexBuffer(indices, this->indicesCount);

    // Unbund VAO
    glBindVertexArray(0);

    // Load and compile shader program
    if(this->useNormals) this->shaderProgram = createShaderProgram("./shaders/vertex/vs_normals.glsl", "./shaders/fragment/fs_normals.glsl");
    else this->shaderProgram = createShaderProgram("./shaders/vertex/vs_base.glsl", "./shaders/fragment/fs_base.glsl");
    
    glUniformBlockBinding(this->shaderProgram, 0, 0);

    // Free
    free(vertexPositions);
    free(vertexColors);
    free(vertexNormals);
    free(indices);
}

Renderable::~Renderable() {
    delete this->vbp;
    delete this->vbc;
    if(this->useNormals) delete this->vbn;

    delete this->vbi;
    delete this->ib;

    glDeleteProgram(this->shaderProgram);
}

unsigned int Renderable::addInstance(glm::mat4 modelMatrix) {
    glm::mat4* modelMatrixp = (glm::mat4*) malloc(sizeof(glm::mat4));
    *modelMatrixp = modelMatrix;

    unsigned int instanceID = this->modelMatrices->add(modelMatrixp);
    this->updatedData = true;

    return instanceID;
}

void Renderable::removeInstance(unsigned int instanceID) {
    free(this->modelMatrices->removeById(instanceID));
    this->updatedData = true;
}

void Renderable::updateInstance(glm::mat4 modelMatrix, unsigned int instanceID) {
    glm::mat4* modelMatrixp = (glm::mat4*) malloc(sizeof(glm::mat4));
    *modelMatrixp = modelMatrix;

    free(this->modelMatrices->updateById(instanceID, modelMatrixp));
    this->updatedData = true;
}

void Renderable::updateBuffer() {
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