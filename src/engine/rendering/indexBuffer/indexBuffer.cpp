#include <iostream>
#include <stdlib.h>

#include <glew.h>
#include <glfw3.h>

#include "indexBuffer.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count) 
    : count(count)
{
    glGenBuffers(1, &rendererID); // Create a buffer in memory
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID); // Say we are using this buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW); // Fill the buffer with data
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &rendererID); // Free memory by deleting buffer
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, rendererID); // Say we are using this buffer
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Say we are using this buffer
}