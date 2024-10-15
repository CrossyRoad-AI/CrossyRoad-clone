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
        this->restart();

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

    return this->count ++;
}

void DoubleLinkedList::removeByIndex(const unsigned int index) {
    this->restart();

    if(index == 0) {
        this->firstElement = this->firstElement->next;
        this->firstElement->prev = nullptr;
        
        free(this->currentElement->content);
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
    this->restart();
    while(this->currentElementIndex < index) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    free(this->currentElement->content);
    this->currentElement->content = element;
}

void* DoubleLinkedList::getCurrentNext() {
    if(this->currentElement) {
        void* current = this->currentElement->content;

        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;

        return current;
    } return NULL;
}

void* DoubleLinkedList::getCurrentPrev() {
    if(this->currentElement) {
        void* current = this->currentElement->content;

        this->currentElement = this->currentElement->prev;
        this->currentElementIndex --;

        return current;
    } return NULL;
}

void* DoubleLinkedList::getElementById(unsigned int id) {
    this->restart();
    while(this->currentElement->id < id) {
        this->currentElement = this->currentElement->next;
        this->currentElementIndex ++;
    }

    return this->currentElement->content;
}