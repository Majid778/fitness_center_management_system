
#ifndef FITNESSMGMTSYSTEM_DLINKEDLIST_H
#define FITNESSMGMTSYSTEM_DLINKEDLIST_H

#include <stdlib.h>
#include <iostream>

using std::string;
using std::cout;


template<typename T>
class Node {
public:
    T data; // the data/element stored in the current Node
    Node<T> *next; // pointer to the next Node in the list
    Node<T> *prev; // pointer to the previous Node in the list
    Node(T dataStored=T()) : data(dataStored), next(NULL),
                             prev(NULL) {}    // constructor that creates a Node object, stores the given data, and sets both previous and next pointers to null
    template<typename U> friend class DLinkedList;
};

template<typename T>
class DLinkedList {
public:
    Node<T> *header;
    Node<T> *trailer;
    int totalItems;

    DLinkedList(); // constructor that creates an empty list
    ~DLinkedList();// destructor of the list
    bool
    isEmpty() const;// is the list empty
    const T &
    getFront() const; //fetches a reference to the element/value stored at the front of our list (does not remove it)
    const T &
    getBack() const; //fetches a reference to the element/value stored at the back/end of our list (does not remove it)
    void addFront(const T &data);// adds the given data/element to the front of the list
    void addBack(const T &data);// adds the given data/element to the back/end of the list
    void removeFront(); //remove the data/element stored at the front of the list
    void removeBack(); //remove the data/element stored at the back of the list
    void remove(Node<T> *toBeRemoved); //remove a specific node from the list
    void insertBetween(Node<T> *before, Node<T> *after,
                       const T &data);//insert a new node containing the given data in between the before and after nodes
};

//constructor
template<typename T>
DLinkedList<T>::DLinkedList() {
    header = new Node<T>();
    trailer = new Node<T>();
    header->next = trailer;
    trailer->prev = header;
    totalItems = 0;
}

//destructor
template<typename T>
DLinkedList<T>::~DLinkedList() {
    // repeatedly remove the first node until the list is empty
    while (!isEmpty()) {
        removeFront();
    }
}

// get a reference to the data stored at the front of the list
template<typename T>
const T& DLinkedList<T>::getFront() const {
    return header->next->data;
}

// get a reference to the data stored at the end of the list
template<typename T>
const T& DLinkedList<T>::getBack() const {
    return trailer->prev->data;
}

// if the header and trailer nodes are directly linked with each other, then the list is empty
template<typename T>
bool DLinkedList<T>::isEmpty() const {
    return header->next == trailer;
}

// insert a new node in between two given nodes.
template<typename T>
void DLinkedList<T>::insertBetween(Node<T> *before, Node<T> *after, const T &data) {
    Node<T> *newNode = new Node<T>(data);
    before->next = newNode;
    newNode->prev = before;
    after->prev = newNode;
    newNode->next = after;
    totalItems++;
}

// insert a new node at the start of our list.
template<typename T>
void DLinkedList<T>::addFront(const T &data) {
    insertBetween(header, header->next, data);
}

// insert a new node at the end of our list.
template<typename T>
void DLinkedList<T>::addBack(const T &data) {
    insertBetween(trailer->prev, trailer, data);
}

// remove a specific node from the list
template<typename T>
void DLinkedList<T>::remove(Node<T> *toBeRemoved) {
    // fetch nodes before and after the node to be removed
    Node<T> *before = toBeRemoved->prev;
    Node<T> *after = toBeRemoved->next;
    // bypass the node by linking these two nodes together
    before->next = after;
    after->prev = before;
    // delete the node
    delete toBeRemoved;
    totalItems--;
}

// remove the last node from the list
template<typename T>
void DLinkedList<T>::removeBack() {
    remove(trailer->prev);
}

// remove the first node from the list
template<typename T>
void DLinkedList<T>::removeFront() {
    remove(header->next);
}

#endif //FITNESSMGMTSYSTEM_DLINKEDLIST_H
