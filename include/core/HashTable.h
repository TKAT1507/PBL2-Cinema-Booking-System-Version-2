#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "core/DynamicArray.h"
#include <string>
#include <functional>
#include <type_traits>
#include <utility>

template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;

    HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V>
class HashTable {
private:
    HashNode<K, V>** m_table;
    int m_capacity;
    int m_size;
    constexpr static float LOAD_FACTOR = 0.75f;

    size_t hashFunc(const K& key) const {
        if constexpr (std::is_same_v<K, std::string>) {
            // DJB2 Hash Algorithm for std::string
            size_t hash = 5381;
            for (char c : key) {
                hash = ((hash << 5) + hash) + static_cast<unsigned char>(c);
            }
            return hash;
        } else {
            return std::hash<K>{}(key);
        }
    }

    int getBucketIndex(const K& key, int capacity) const {
        return static_cast<int>(hashFunc(key) % static_cast<size_t>(capacity));
    }

    void rehash(int newCapacity) {
        if (newCapacity <= m_capacity) return;

        HashNode<K, V>** newTable = new HashNode<K, V>*[newCapacity]();

        for (int i = 0; i < m_capacity; ++i) {
            HashNode<K, V>* current = m_table[i];
            while (current != nullptr) {
                HashNode<K, V>* next = current->next;
                int newIdx = getBucketIndex(current->key, newCapacity);

                current->next = newTable[newIdx];
                newTable[newIdx] = current;

                current = next;
            }
        }

        delete[] m_table;
        m_table = newTable;
        m_capacity = newCapacity;
    }

public:
    HashTable(int initialCapacity = 16) : m_capacity(initialCapacity), m_size(0) {
        if (m_capacity < 4) m_capacity = 4;
        m_table = new HashNode<K, V>*[m_capacity]();
    }

    ~HashTable() {
        clear();
        delete[] m_table;
    }

    HashTable(const HashTable& other) : m_capacity(other.m_capacity), m_size(0) {
        m_table = new HashNode<K, V>*[m_capacity]();
        for (int i = 0; i < other.m_capacity; ++i) {
            HashNode<K, V>* current = other.m_table[i];
            while (current != nullptr) {
                put(current->key, current->value);
                current = current->next;
            }
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            clear();
            delete[] m_table;

            m_capacity = other.m_capacity;
            m_size = 0;
            m_table = new HashNode<K, V>*[m_capacity]();

            for (int i = 0; i < other.m_capacity; ++i) {
                HashNode<K, V>* current = other.m_table[i];
                while (current != nullptr) {
                    put(current->key, current->value);
                    current = current->next;
                }
            }
        }
        return *this;
    }

    HashTable(HashTable&& other) noexcept 
        : m_table(other.m_table), m_capacity(other.m_capacity), m_size(other.m_size) {
        other.m_table = nullptr;
        other.m_capacity = 0;
        other.m_size = 0;
    }

    HashTable& operator=(HashTable&& other) noexcept {
        if (this != &other) {
            clear();
            delete[] m_table;

            m_table = other.m_table;
            m_capacity = other.m_capacity;
            m_size = other.m_size;

            other.m_table = nullptr;
            other.m_capacity = 0;
            other.m_size = 0;
        }
        return *this;
    }

    int size() const { return m_size; }
    int capacity() const { return m_capacity; }
    bool isEmpty() const { return m_size == 0; }

    bool put(const K& key, const V& value) {
        if (m_size >= m_capacity * LOAD_FACTOR) {
            rehash(m_capacity * 2);
        }

        int index = getBucketIndex(key, m_capacity);
        HashNode<K, V>* current = m_table[index];

        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return false; // Updated existing key
            }
            current = current->next;
        }

        // New insertion
        HashNode<K, V>* newNode = new HashNode<K, V>(key, value);
        newNode->next = m_table[index];
        m_table[index] = newNode;
        m_size++;
        return true;
    }

    V* get(const K& key) {
        if (m_table == nullptr || m_capacity == 0) return nullptr;
        int index = getBucketIndex(key, m_capacity);
        HashNode<K, V>* current = m_table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    const V* get(const K& key) const {
        if (m_table == nullptr || m_capacity == 0) return nullptr;
        int index = getBucketIndex(key, m_capacity);
        HashNode<K, V>* current = m_table[index];

        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool containsKey(const K& key) const {
        return get(key) != nullptr;
    }

    bool remove(const K& key) {
        if (m_table == nullptr || m_capacity == 0) return false;
        int index = getBucketIndex(key, m_capacity);
        HashNode<K, V>* current = m_table[index];
        HashNode<K, V>* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    m_table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                m_size--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    void clear() {
        if (m_table == nullptr) return;
        for (int i = 0; i < m_capacity; ++i) {
            HashNode<K, V>* current = m_table[i];
            while (current != nullptr) {
                HashNode<K, V>* next = current->next;
                delete current;
                current = next;
            }
            m_table[i] = nullptr;
        }
        m_size = 0;
    }

    DynamicArray<K> keys() const {
        DynamicArray<K> res;
        if (m_table == nullptr) return res;
        for (int i = 0; i < m_capacity; ++i) {
            HashNode<K, V>* current = m_table[i];
            while (current != nullptr) {
                res.push_back(current->key);
                current = current->next;
            }
        }
        return res;
    }

    DynamicArray<V> values() const {
        DynamicArray<V> res;
        if (m_table == nullptr) return res;
        for (int i = 0; i < m_capacity; ++i) {
            HashNode<K, V>* current = m_table[i];
            while (current != nullptr) {
                res.push_back(current->value);
                current = current->next;
            }
        }
        return res;
    }
};

#endif // HASH_TABLE_H
