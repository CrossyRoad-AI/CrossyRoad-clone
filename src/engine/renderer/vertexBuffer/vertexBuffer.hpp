#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

class VertexBuffer {
    private:
        unsigned int rendererID;
        
    public:
        VertexBuffer(const void* data, const unsigned int size, unsigned int drawType); // Constructor
        ~VertexBuffer(); // Destructor

        void bind();
        void unbind();

        void update(const void* data, const unsigned int size);
};

#endif