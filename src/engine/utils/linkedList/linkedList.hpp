#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

typedef struct LinkedListt_ {
    unsigned int id;

    void* content;
    struct LinkedListt_* next;
} LinkedListt;

class LinkedList {
    private:
        LinkedListt* firstElement;

        unsigned int currentElementIndex;
        LinkedListt* currentElement;

        unsigned int count;

    public:
        LinkedList();
        ~LinkedList();

        unsigned int add(void* element);
        void update(const unsigned int index, void* element);

        void removeByIndex(const unsigned int index);
        void removeById(const unsigned int id);

        // Getters and setters
        inline void* first() { return this->firstElement->content; }
        void restart() { this->currentElement = this->firstElement; this->currentElementIndex = 0; }

        void* getCurrent();
        void* getElementById(unsigned int index);

        inline unsigned int getCount() { return this->count; }
};

#endif