#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../../rendering/vertexBuffer/vertexBuffer.hpp"
#include "../../rendering/indexBuffer/indexBuffer.hpp"

#include "../../utils/linkedlist/linkedlist.hpp"

class Renderable {
    private:
        unsigned int vao;
        unsigned int indicesCount;

        bool updatedData;
        LinkedList* modelMatrices;
        LinkedList* normalMatrices;

        VertexBuffer* vbp;
        VertexBuffer* vbc;
        VertexBuffer* vbn;
        VertexBuffer* vbi;
        VertexBuffer* vbnm;
        IndexBuffer* ib;

        unsigned int shaderProgram;

        static inline void freeCallback(void* element) { free(element); }; 

    public:
        Renderable(std::string modelFilename); // Constructor
        ~Renderable(); // Destructor

        unsigned int addInstance(glm::mat4 modelMatrix);
        void removeInstance(unsigned int instanceID);

        void updateBuffer();
        void updateInstance(glm::mat4 modelMatrix, unsigned int instanceID);

        void render();
};

#endif