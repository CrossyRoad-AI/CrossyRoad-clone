#include <iostream>
#include "stdlib.h"

#include "doubleLinkedList.hpp"

DoubleLinkedList::DoubleLinkedList()
    : firstElement(nullptr), currentElementIndex(0), currentElement(nullptr), count(0)
{

}

unsigned int DoubleLinkedList::add(void* element) {
    DoubleLinkedListt_* listElement = (DoubleLinkedListt_*) malloc(sizeof(DoubleLinkedListt_));
    listElement->content = element;
    listElement->next = nullptr;
    listElement->prev = nullptr;

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
        listElement->prev = this->currentElement;
        listElement->id = this->currentElement->id + 1;

        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    this->count ++;
    return this->count - 1;
}

void DoubleLinkedList::remove(const unsigned int index) {
    this->currentElement = this->firstElement;
    this->currentElementIndex = 0;

    if(index == 0) {
        this->firstElement = this->firstElement->next;
        this->firstElement->prev = nullptr;
        
        free(this->currentElement);
    } else {
        while(this->currentElementIndex < index - 1) {
            this->currentElement = this->currentElement->next;
            this->currentElementIndex ++;
        }

        DoubleLinkedListt_* next = this->currentElement->next->next;

        free(this->currentElement->next->content);
        free(this->currentElement->next);

        this->currentElement->next = next;
        this->currentElement->next->prev = this->currentElement;
    }

    this->count --;
}

void DoubleLinkedList::update(const unsigned int index, void* element) {
    this->currentElement = this->firstElement;
    this->currentElementIndex = 0;

    while(this->currentElementIndex < index) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    free(this->currentElement->content);
    this->currentElement->content = element;
}

void* DoubleLinkedList::getCurrentNext() {
    void* current = this->currentElement->content;

    if(this->currentElement->next) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    return current;
}

void* DoubleLinkedList::getCurrentPrev() {
    void* current = this->currentElement->content;

    if(this->currentElement->prev) {
        this->currentElement = this->currentElement->prev;
        this->currentElementIndex --;
    }

    return current;
}

void* DoubleLinkedList::getElementWithId(unsigned int id) {
    this->currentElement = this->firstElement;
    this->currentElementIndex = 0;

    while(this->currentElement->id < id) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    return this->currentElement->content;
}