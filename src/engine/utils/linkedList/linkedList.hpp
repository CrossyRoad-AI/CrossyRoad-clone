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
        LinkedListt* currentElement;

        unsigned int count;

        void (*freeCallback)(void* element);

    public:
        LinkedList(void (*freeCallback)(void* element));
        ~LinkedList();

        unsigned int add(void* element);

        void updateByIndex(const unsigned int index, void* element);
        void updateById(const unsigned int instanceId, void* element);

        void removeByIndex(const unsigned int index);
        void removeById(const unsigned int id);

        // Getters and setters
        inline void* first() { return this->firstElement->content; }

        void restart() { this->currentElement = this->firstElement; }
        void* getCurrent();

        void* getElementById(unsigned int index);

        inline unsigned int getCount() { return this->count; }
};

#endif