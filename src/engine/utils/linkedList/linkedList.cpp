#include <iostream>
#include "stdlib.h"

#include "linkedList.hpp"

LinkedList::LinkedList()
    : firstElement(nullptr), currentElement(nullptr), count(0)
{}

LinkedList::~LinkedList() {}

unsigned int LinkedList::add(void* element) {
    LinkedListt* listElement = (LinkedListt*) malloc(sizeof(LinkedListt));
    listElement->content = element;
    listElement->next = nullptr;

    this->count ++;

    if(!this->firstElement) {
        this->firstElement = listElement;
        this->firstElement->id = 0;

        return 0;
    } else {
        LinkedListt* current = this->firstElement;
        while(current->next) current = current->next;

        current->next = listElement;
        listElement->id = current->id + 1;

        return listElement->id;
    }
}

void* LinkedList::update(const unsigned int index, void* element) {
    LinkedListt* current = this->firstElement;
    unsigned int i = 0;

    while(i ++ < index) current = current->next;

    void* contentToFree = current->content;
    current->content = element;

    return contentToFree;
}

void* LinkedList::updateById(const unsigned int instanceId, void* element) {
    LinkedListt* current = this->firstElement;
    while(current->id < instanceId) current = current->next;

    void* contentToFree = current->content;
    current->content = element;

    return contentToFree;
}

void* LinkedList::removeByIndex(const unsigned int index) {
    LinkedListt* current = this->firstElement;
    void* contentToFree;

    if(index == 0) {
        this->firstElement = this->firstElement->next;

        contentToFree = current->content;
        free(current);
    } else {
        unsigned int i = 0;
        while(i ++ < index - 1) current = current->next;
        LinkedListt* next = current->next->next;

        contentToFree = current->next->content;
        free(current->next);

        current->next = next;
    }

    this->count --;
    return contentToFree;
}

void* LinkedList::removeById(const unsigned int id) {
    LinkedListt* current = this->firstElement;
    unsigned int i = 0;
    void* contentToFree;

    while(current->id < id) {
        current = current->next;
        i ++;
    }

    if(i == 0) {
        this->firstElement = this->firstElement->next;

        contentToFree = current->content;
        free(current);
    } else {
        LinkedListt* next = current->next->next;

        contentToFree = current->next->content;
        free(current->next);

        current->next = next;
    }

    this->count --;
    return contentToFree;
}

void* LinkedList::getCurrent() {
    if(this->currentElement) {
        void* content = this->currentElement->content;
        this->currentElement = this->currentElement->next;

        return content;
    } return NULL;
}

void* LinkedList::getElementById(unsigned int id) {
    LinkedListt* current = this->firstElement;
    while(current->id < id) current = current->next;

    return current->content;
}