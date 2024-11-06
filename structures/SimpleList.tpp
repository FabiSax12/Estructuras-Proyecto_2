#ifndef SIMPLELIST_TPP
#define SIMPLELIST_TPP

#include "SimpleList.h"

using namespace std;

template <typename T>
SimpleList<T>::SimpleList() : head(nullptr), length(0) {}

template <typename T>
SimpleList<T>::~SimpleList() {
    clear();
}

template <typename T>
void SimpleList<T>::add(const T& item) {
    T* newNode = new T(item);
    newNode->next = head;
    head = newNode;
    ++length;
}

template <typename T>
void SimpleList<T>::add(T* item) {
    T* newNode = item;
    newNode->next = head;
    head = newNode;
    ++length;
}

template <typename T>
bool SimpleList<T>::remove(T& item) {
    if (!head) return false;

    if (head->name == item.name) {
        T* removedNode = head;
        head = head->next;
        --length;
        return true;
    }

    T* current = head;
    while (current != nullptr) {
        if (current->name == item.name) {
            T* removedNode = current->next;
            current->next = current->next->next;
            --length;
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename T>
bool SimpleList<T>::removeByIndex(int index) {
    if (index < 0) index = length + index;

    if (index == 0) {
        head = head->next;
        --length;
        return true;
    }

    T* currentNode = head;
    int counter = 0;
    while (currentNode && counter < index-1) {
        currentNode = currentNode->next;
        ++counter;
    }
    if (currentNode == nullptr) throw std::runtime_error("Invalid index");

    currentNode->next = currentNode->next->next;
    --length;
    return true;
}


template <typename T>
void SimpleList<T>::clear() {
    while (head) {
        T* temp = head;
        head = head->next;
        delete temp;
    }
    length = 0;
}

template <typename T>
void SimpleList<T>::print() const {
    for (const T& item : *this) {
        std::cout << item << std::endl;
    }
}

template <typename T>
T* SimpleList<T>::get(int index) const {
    if (index < 0) index = length + index;
    T* currentNode = head;
    int counter = 0;
    while (currentNode && counter < index) {
        currentNode = currentNode->next;
        ++counter;
    }
    if (currentNode == nullptr) throw std::runtime_error("Invalid index");

    return currentNode;
}

template<typename T>
int SimpleList<T>::getLength() const {
    return this->length;
}


template<typename T>
std::string SimpleList<T>::toString() const {
    std::string rep = "{ ";
    T* currentNode = head;
    do {
        rep += currentNode->name;
        if (currentNode->next && currentNode->next != head) rep += ", ";
        currentNode = currentNode->next;
    } while (currentNode && currentNode != head);

    if (rep.length() > 70) {
        for (int i = 0; i < rep.length(); i++) {
            if (rep.compare(i, 1, ",")) {
                rep.insert(i+1, "\n");
            }
        }
    }

    rep += " }";
    return rep;
}

template<typename T>
std::string SimpleList<T>::toString(const std::function<std::string(const T&)>& fieldExtractor) const {
    if (head == nullptr) return "{ }";
    std::string rep = "{ ";
    T* currentNode = head;
    do {
        rep += fieldExtractor(currentNode);
        if (currentNode->next && currentNode->next != head) rep += ", ";
        currentNode = currentNode->next;
    } while (currentNode && currentNode != head);

    if (rep.length() > 70) {
        for (int i = 0; i < rep.length(); i++) {
            if (rep.at(i) == ',' || rep.at(i) == '{') {
                rep.insert(i + 1, "\n\t");
            }
        }
        rep.append("\n");
    }

    rep += " }";
    return rep;
}

template <typename T>
void SimpleList<T>::printRecursive() const {
    printRecursive(head);
}

template <typename T>
void SimpleList<T>::printRecursive(T* node) const {
    if (node == nullptr) return;
    std::cout << node->toString() << std::endl;
    printRecursive(node->next);
}

#endif // SIMPLELIST_TPP