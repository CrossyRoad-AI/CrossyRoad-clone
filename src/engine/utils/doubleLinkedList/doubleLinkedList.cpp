#include <iostream>
#include "stdlib.h"

#include "doubleLinkedList.hpp"

DoubleLinkedList::DoubleLinkedList()
    : firstElement(nullptr), currentElementIndex(0), currentElement(nullptr), count(0)
{}

DoubleLinkedList::~DoubleLinkedList() {}

unsigned int DoubleLinkedList::add(void* element) {
    DoubleLinkedListt* listElement = (DoubleLinkedListt*) malloc(sizeof(DoubleLinkedListt));
    listElement->content = element;
    listElement->next = nullptr;
    listElement->prev = nullptr;

    this->count ++;

    if(!this->firstElement) {
        this->firstElement = listElement;
        this->firstElement->id = 0;

        return 0;
    } else {
        DoubleLinkedListt* current = this->firstElement;
        while(current->next) current = current->next;

        current->next = listElement;
        listElement->prev = current;
        listElement->id = current->id + 1;

        return listElement->id;
    }
}

void* DoubleLinkedList::removeByIndex(const unsigned int index) {
    DoubleLinkedListt* current = this->firstElement;
    unsigned int i = 0;
    void* contentToFree;

    if(index == 0) {
        contentToFree = this->firstElement->content;
        this->firstElement = this->firstElement->next;
        
        free(this->firstElement->prev);
        this->firstElement->prev = nullptr;
    } else {
        while(i ++ < index - 1) current = current->next;
        DoubleLinkedListt* next = current->next->next;

        contentToFree = current->next->content;
        free(current->next);

        current->next = next;
        current->next->prev = current;
    }

    this->count --;
    return contentToFree;
}

void* DoubleLinkedList::update(const unsigned int index, void* element) {
    DoubleLinkedListt* current = this->firstElement; unsigned int i = 0;
    while(i ++ < index) current = current->next;

    void* contentToFree = current->content;
    current->content = element;

    return contentToFree;
}

void* DoubleLinkedList::getCurrentNext() {
    if(this->currentElement) {
        void* content = this->currentElement->content;
        this->currentElement = this->currentElement->next;

        return content;
    } return NULL;
}

void* DoubleLinkedList::getCurrentPrev() {
    if(this->currentElement) {
        void* current = this->currentElement->content;
        this->currentElement = this->currentElement->prev;

        return current;
    } return NULL;
}

void* DoubleLinkedList::getElementByIndex(unsigned int index) {
    DoubleLinkedListt* current = this->firstElement; unsigned int i = 0;
    while(i ++ < index) current = current->next;

    return current->content;
}

void* DoubleLinkedList::getElementById(unsigned int id) {
    DoubleLinkedListt* current = this->firstElement;
    while(current->id < id) current = current->next;

    return current->content;
}