#include <iostream>
#include "stdlib.h"

#include "linkedList.hpp"

LinkedList::LinkedList(void (*freeCallback)(void* element))
    : firstElement(nullptr), currentElement(nullptr), count(0)
{
    this->freeCallback = freeCallback;
}

LinkedList::~LinkedList() {
    LinkedListt* current = this->firstElement;
    while(current) {
        LinkedListt* next = current->next;

        this->freeCallback(current->content);
        free(current);

        current = next;
    }
}

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

void LinkedList::updateByIndex(const unsigned int index, void* element) {
    LinkedListt* current = this->firstElement;

    unsigned int i = 0;
    while(i ++ < index) current = current->next;

    this->freeCallback(current->content);
    current->content = element;
}

void LinkedList::updateById(const unsigned int instanceId, void* element) {
    LinkedListt* current = this->firstElement;
    while(current->id < instanceId) current = current->next;

    this->freeCallback(current->content);
    current->content = element;
}

void LinkedList::removeByIndex(const unsigned int index) {
    LinkedListt* current = this->firstElement;
    this->count --;

    if(index == 0) {
        this->firstElement = this->firstElement->next;

        this->freeCallback(current->content);
        free(current);
    } else {
        unsigned int i = 0;
        while(i ++ < index - 1) current = current->next;
        LinkedListt* next = current->next->next;

        this->freeCallback(current->next->content);
        free(current->next);

        current->next = next;
    }
}

void LinkedList::removeById(const unsigned int id) {
    LinkedListt* current = this->firstElement;
    this->count --;

    unsigned int i = 0;
    while(current->id < id) {
        current = current->next;
        i ++;
    }

    if(i == 0) {
        this->firstElement = this->firstElement->next;

        this->freeCallback(current->content);
        free(current);
    } else {
        LinkedListt* next = current->next->next;

        this->freeCallback(current->next->content);
        free(current->next);

        current->next = next;
    }
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