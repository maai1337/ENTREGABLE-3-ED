#pragma once
#include <vector>
#include <list>
#include <string>

// 1. Función Hash para 'user_id' (Algoritmo Polinomial Multiplicativo)
struct HashUserID {
    size_t operator()(const std::string& key) const {
        size_t hash = 0;
        for (char c : key) {
            hash = hash * 31 + c;
        }
        return hash;
    }
};

// 2. Función Hash para 'user_screen_name' (Algoritmo DJB2)
struct HashScreenName {
    size_t operator()(const std::string& key) const {
        size_t hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash;
    }
};

// Se usa template para funcion hash dependiendo de la key
template <typename HashFunc>
class HashTableChaining {
private:
    struct HashNode {
        std::string key;
        int count;
    };

    std::vector<std::list<HashNode>> table;
    size_t numBuckets;
    size_t numElements;
    HashFunc hashFunction; // Instancia de la funcion hash (DJB2 o la de IDs)

public:
    HashTableChaining(size_t size = 200003) : numBuckets(size), numElements(0) {
        table.resize(numBuckets);
    }

    void insert(const std::string& key) {
        size_t index = hashFunction(key) % numBuckets;

        // if(k está en H) H[k] = H[k] + 1
        for (auto& node : table[index]) {
            if (node.key == key) {
                node.count++;
                return;
            }
        }

        // else H[k] = 1
        table[index].push_back({key, 1});
        numElements++;
    }

    int get(const std::string& key) {
        size_t index = hashFunction(key) % numBuckets;
        for (const auto& node : table[index]) {
            if (node.key == key) {
                return node.count;
            }
        }
        return 0;
    }

    size_t size() const { return numElements; }

    size_t getBuckets() const { return numBuckets; }

    size_t memoryBytes() const {
        // Memoria del vector de listas
        size_t mem = numBuckets * sizeof(std::list<HashNode>);
        // Memoria de cada nodo en las listas
        for (size_t i = 0; i < numBuckets; ++i) {
            for (const auto& node : table[i]) {
                // Overhead del nodo de la lista (prev + next punteros)
                mem += 2 * sizeof(void*);
                // Tamaño del HashNode (string + int)
                mem += sizeof(HashNode);
                // Memoria del heap del string (si excede SSO)
                mem += node.key.capacity();
            }
        }
        return mem;
    }
};