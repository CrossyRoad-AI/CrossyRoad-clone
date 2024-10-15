#include <iostream>
#include "stdlib.h"

#include "linkedList.hpp"

LinkedList::LinkedList()
    : firstElement(nullptr), currentElementIndex(0), currentElement(nullptr), count(0)
{

}

unsigned int LinkedList::add(void* element) {
    LinkedListt* listElement = (LinkedListt*) malloc(sizeof(LinkedListt));
    listElement->content = element;
    listElement->next = nullptr;

    if(!this->firstElement) {
        this->firstElement = listElement;
        this->firstElement->id = 0;
    } else {
        this->currentElement = this->firstElement;
        this->currentElementIndex = 0;

        while(this->currentElement->next) {
            this->currentElement = this->currentElement->next;
            this->currentElementIndex ++;
        }

        this->currentElement->next = listElement;
        listElement->id = this->currentElement->id + 1;

        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    this->count ++;
    return this->count - 1;
}

void LinkedList::remove(const unsigned int index) {
    this->currentElement = this->firstElement;
    this->currentElementIndex = 0;

    if(index == 0) {
        this->firstElement = this->firstElement->next;
        free(this->currentElement);
    } else {
        while(this->currentElementIndex < index - 1) {
            this->currentElement = this->currentElement->next;
            this->currentElementIndex ++;
        }

        LinkedListt* next = this->currentElement->next->next;

        free(this->currentElement->next->content);
        free(this->currentElement->next);

        this->currentElement->next = next;
    }

    this->count --;
}

void LinkedList::removeById(const unsigned int id) {
    this->currentElement = this->firstElement;
    this->currentElementIndex = 0;

    while(this->currentElement->id < id) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    if(this->currentElementIndex == 0) {
        this->firstElement = this->firstElement->next;
        free(this->currentElement);
    } else {
        LinkedListt* next = this->currentElement->next->next;

        free(this->currentElement->next->content);
        free(this->currentElement->next);

        this->currentElement->next = next;
    }

    this->count --;
}

void LinkedList::update(const unsigned int index, void* element) {
    this->currentElement = this->firstElement;
    this->currentElementIndex = 0;

    while(this->currentElementIndex < index) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    free(this->currentElement->content);
    this->currentElement->content = element;
}

void* LinkedList::getCurrent() {
    if(this->currentElement) {
        void* current = this->currentElement->content;

        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;

        return current;
    } return NULL;
}

void* LinkedList::getElementWithId(unsigned int id) {
    this->currentElement = this->firstElement;
    this->currentElementIndex = 0;

    while(this->currentElement->id < id) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    return this->currentElement->content;
}