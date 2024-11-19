#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

class IndexBuffer {
    private:
        unsigned int rendererID;
        unsigned int count;
    public:
        IndexBuffer(const unsigned int* data, const unsigned int count); // Constructor
        ~IndexBuffer(); // Destructor

        void bind() const;
        void unbind() const;

        inline unsigned int getCount() { return count; };
};

#endif