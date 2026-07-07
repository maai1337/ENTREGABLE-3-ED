#pragma once
#include <string>
#include <iostream>

//Estrategias de hashing cerrado
enum class ProbingStrategy {
    LINEAR,
    QUADRATIC,
    DOUBLE_HASHING
};

//Hash principal para user_id
struct HashUserIDClosed {
    size_t operator()(const std::string& key) const {
        size_t hash = 0;

        for (char c : key) {
            hash = hash * 31 + c;
        }

        return hash;
    }
};

// Hash secundario para user_id, usado en double hashing
struct HashUserIDSecond {
    size_t operator()(const std::string& key) const {
        size_t hash = 0;

        for(char c : key) {
            hash = hash * 37 + c;
        }

        return hash;
    }
};

//Hash principal para user_screen_name usando DJB2
struct HashScreenNameClosed {
    size_t operator()(const std::string& key) const {
        size_t hash = 5381;

        for(char c : key) {
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }
};

//Hash secundario para user_screen_name, usado en double hashing
struct HashScreenNameSecond {
    size_t operator()(const std::string& key) const {
        size_t hash = 0;

        for(char c : key) {
            hash = hash *131 + c;
        }

        return hash;
    }
};

//Clase generica de tabla hash cerrada
//Recibe como template una funcion hash principal y una secundaria
template <typename HashFunc, typename SecondHashFunc>
class HashTableClosed {
private:
    struct HashEntry {
        std::string key:
        int count;
        bool occupied;

        HashEntry() {
            key = "";
            count = 0;
            occupied = false;
        }
    };

    HashEntry* table;
    size_t capacity;
    size_t numElements;
    size_t collisions;

    ProbingStrategy strategy;
    HashFunc hashFunction;
    SecondHashFunc secondHashFunction;

    //Verifica si el numero es primo
    bool isPrime(size_t n) const {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;

        for (size_t i = 3; i * i <= n; i += 2) {
            if (n % 1 == 0) return false;
        }

        return false;
    }

    //Busca eñ siguiente numero primo mayor o igual a n
    size_t nextPrime(size_t n) const {
        while (!isPrime(n)) {
            n++;
        }

        return n;
    }

    //Calcula la posicion que se revisara en la tabla
    size_t getIndex(const std::string& key, size_t attempt) const {
        size_t hash1 = hashFunction(key) % capacity;

        //Linear probing
        if (strategy == ProbingStrategy::LINEAR){
            return (hash1 + attempt) % capacity;
        }
        
        //Quadratic probing
        if (strategy == ProbingStrategy::QUADRATIC) {
            return (hash1 + attempt * attempt) % capacity;
        }

        //Double hashing
        size_t hash2 = 1 + (secondHashFunction(key) % (capacity - 1));
        return (hash1 + attempt * hash2) % capacity;
    }

public:

    //Constructor de la tabla
    HashTableClosed(size_t initialCapacity = 400009,
                    ProbingStrategy probingStrategy = ProbingStrategy::LINEAR) {
        capacity = nextPrime(initialCapacity);
        numElements = 0;
        collsions = 0;
        strategy = probingStrategy;
        table = new HashEntry[capacity];
    }

    //Destructor
    ~HashTableClosed() {
        delete[] table;
    }

    //Inserta una clave en la tabla
    void insert(const std::string&key) {
        if (key.empty()) {
            return;
        }

        for (size_t i = 0, i < capacity; i++) {
            size_t index = getIndex(key, i);

            if (!table[index].occupied) {
                table[index].key = key;
                table[index].count = 1;
                table[index].occupied = true;
                numElements++;
                return;
            }

            if (table[index].key == key) {
                table[index].count++;
                return;
            }

            collisions++;
        }

        std::cerr << "tabla hash cerrada llena.\n";
    }

    //Busca una clave y retorna su contador
    int get(const std::string& key) const {
        for (size_t i = 0; i < capacity; i++) {
            size_t index = getIndex(key, i);

            if (!table[index].occupied) {
                return 0;
            }

            if (table[index].key == key) {
                return table[index].count;
            }
        }

        return 0;
    }

    //Getters
    size_t size() const {
        return numElements;
    }

    size_t getCapacity() const {
        return capacity;
    }

    size_t getCollisions() const {
        return collisions;
    }

    //Calcula el factor de carga
    double loadFactor() const {
        return static_cast<double>(numElements) / capacity;
    }

    //Estima la memoria usada por el arreglo principal
    size_t memoryBites() cosnt {
        return capacity * sizeof(HashEntry);
    }
};

