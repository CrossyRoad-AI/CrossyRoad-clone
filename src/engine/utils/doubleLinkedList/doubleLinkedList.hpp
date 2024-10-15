#ifndef DOUBLELINKEDLIST_HPP
#define DOUBLELINKEDLIST_HPP

typedef struct DoubleLinkedListt_ {
    unsigned int id;

    void* content;
    struct DoubleLinkedListt_* next;
    struct DoubleLinkedListt_* prev;
} DoubleLinkedListt;

class DoubleLinkedList {
    private:
        DoubleLinkedListt* firstElement;

        unsigned int currentElementIndex;
        DoubleLinkedListt* currentElement;

        unsigned int count;

    public:
        DoubleLinkedList();

        unsigned int add(void* element);
        void remove(const unsigned int index);
        void update(const unsigned int index, void* element);

        // Getters and setters
        inline void* first() { return this->firstElement->content; }

        void restart() { this->currentElement = this->firstElement; this->currentElementIndex = 0; }

        void* getCurrentNext();
        void* getCurrentPrev();
        void* getElementWithId(unsigned int index);

        inline unsigned int getCount() { return this->count; }
};

#endif