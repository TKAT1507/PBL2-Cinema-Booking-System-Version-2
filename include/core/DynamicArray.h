#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <stdexcept>
#include <algorithm>

template <typename T>
class DynamicArray {
private:
    T* data;
    int m_size;
    int m_capacity;

public:
    DynamicArray() : m_size(0), m_capacity(4) {
        data = new T[m_capacity];
    }

    ~DynamicArray() {
        delete[] data;
    }

    DynamicArray(const DynamicArray& other) : m_size(other.m_size), m_capacity(other.m_capacity) {
        data = new T[m_capacity];
        for (int i = 0; i < m_size; ++i) {
            data[i] = other.data[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            delete[] data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            data = new T[m_capacity];
            for (int i = 0; i < m_size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    DynamicArray(DynamicArray&& other) noexcept : data(other.data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }

    int size() const { return m_size; }
    int capacity() const { return m_capacity; }
    bool isEmpty() const { return m_size == 0; }

    T& operator[](int index) {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }

    void resize(int newCapacity) {
        if (newCapacity <= m_capacity) return;
        T* newData = new T[newCapacity];
        for (int i = 0; i < m_size; ++i) {
            newData[i] = std::move(data[i]);
        }
        delete[] data;
        data = newData;
        m_capacity = newCapacity;
    }

    void push_back(const T& value) {
        if (m_size == m_capacity) {
            resize(m_capacity * 2);
        }
        data[m_size++] = value;
    }

    void insert(int index, const T& value) {
        if (index < 0 || index > m_size) {
            throw std::out_of_range("Index out of bounds");
        }
        if (m_size == m_capacity) {
            resize(m_capacity * 2);
        }
        for (int i = m_size; i > index; --i) {
            data[i] = std::move(data[i - 1]);
        }
        data[index] = value;
        m_size++;
    }

    void removeAt(int index) {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of bounds");
        }
        for (int i = index; i < m_size - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        m_size--;
    }

    void clear() {
        m_size = 0;
    }

    int find(const T& value) const {
        for (int i = 0; i < m_size; ++i) {
            if (data[i] == value) {
                return i;
            }
        }
        return -1;
    }

    bool contains(const T& value) const {
        return find(value) != -1;
    }
};

#endif // DYNAMICARRAY_H
