#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

#include <string>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../renderer/vertexBuffer/vertexBuffer.hpp"
#include "../renderer/indexBuffer/indexBuffer.hpp"

#include "../utils/linkedlist/linkedlist.hpp"

class Renderable {
    private:
        unsigned int vao;
        unsigned int indicesCount;

        bool useNormals;

        bool updatedData;
        LinkedList* modelMatrices;

        VertexBuffer* vbp;
        VertexBuffer* vbc;
        VertexBuffer* vbn;
        VertexBuffer* vbi;
        IndexBuffer* ib;

        unsigned int shaderProgram;

    public:
        // Renderable(std::string modelFilename); // Constructor
        Renderable(std::string modelFilename, bool useNormals); // Constructor
        ~Renderable(); // Destructor

        unsigned int addInstance(glm::mat4 modelMatrix);
        void removeInstance(unsigned int instanceID);

        void updateBuffer();
        void updateInstance(glm::mat4 modelMatrix, unsigned int instanceID);

        void render();
};

#endif