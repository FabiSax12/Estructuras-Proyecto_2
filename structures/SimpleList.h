#ifndef SIMPLELIST_H
#define SIMPLELIST_H

#include <functional>
#include <iostream>

template <typename T>
class SimpleList {
public:
    T* head;
    int length;

    SimpleList();
    ~SimpleList();

    void add(const T& item);
    void add(T* item);
    bool remove(T& item);
    bool removeByIndex(int index);
    void clear();
    void print() const;
    T* get(int index) const;
    [[nodiscard]] int getLength() const;
    [[nodiscard]] std::string toString() const;
    std::string toString(const std::function<std::string(const T&)>& fieldExtractor) const;
    void printRecursive() const;
    void printRecursive(T *node) const;


    struct  Iterator {
        T* current;

        explicit Iterator(T* node) {
            this->current = node;
        }

        T& operator*() const { return *current; }

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

// Include the implementation file
#include "SimpleList.tpp"

#endif // SIMPLELIST_H
