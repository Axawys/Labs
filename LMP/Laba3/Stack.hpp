#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>  // std::move
#include "StackExceptions.hpp"

template<typename T>
class Stack {
private:
    T* data;
    std::size_t capacity;
    std::size_t count;

    void resize() {
        capacity *= 2;
        T* newData = new T[capacity];
        for (std::size_t i = 0; i < count; ++i) {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
    }

public:
    Stack(std::size_t initCapacity = 4)
        : capacity(initCapacity), count(0) {
        data = new T[capacity];
    }

    ~Stack() {
        delete[] data;
    }

    void push(const T& value) {
        if (count == capacity) {
            resize();
        }
        data[count++] = value;
    }

    void pop() {
        if (count == 0) {
            throw StackEmptyException("Попытка pop из пустого стека");
        }
        --count;
    }

    T top() const {
        if (count == 0) {
            throw StackEmptyException("Попытка top из пустого стека");
        }
        return data[count - 1];
    }

    bool empty() const {
        return count == 0;
    }

    std::size_t size() const {
        return count;
    }
};
