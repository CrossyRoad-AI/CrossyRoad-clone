#include <iostream>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>

#include "vertexBuffer.hpp"

VertexBuffer::VertexBuffer(const void* data, const unsigned int size, const unsigned int drawType) {
    glGenBuffers(1, &rendererID); // Create a buffer in memory
    glBindBuffer(GL_ARRAY_BUFFER, rendererID); // Say we are using this buffer
    glBufferData(GL_ARRAY_BUFFER, size, data, drawType); // Fill the buffer with data
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &rendererID); // Free memory by deleting buffer
}

void VertexBuffer::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, rendererID); // Say we are using this buffer
}

void VertexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Say we are using this buffer
}

void VertexBuffer::update(const void* data, const unsigned int size) {
    glBindBuffer(GL_ARRAY_BUFFER, rendererID); // Say we are using this buffer
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // Fill the buffer with data
}