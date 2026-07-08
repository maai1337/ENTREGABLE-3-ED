#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <string>
#include <list>
#include "CSVParser.h"
#include "HashTabClosed.h"
#include "HashTableChaining.h"

using namespace std;
using namespace std::chrono;

// Funcion para calcular la memoria del unordered_map de C++
size_t estimateUnorderedMapMemory(const unordered_map<string, int>& um) {
    // Memoria de los buckets
    size_t mem = um.bucket_count() * sizeof(void*);
    // Memoria de cada nodo en la tabla hash
    for (auto it = um.begin(); it != um.end(); ++it) {
        mem += 2 * sizeof(void*) + sizeof(size_t);
        mem += sizeof(pair<const string, int>);
        mem += it->first.capacity(); // memoria del string
    }
    return mem;
}

void runExperiments() {
    string dataset = "auspol2019";
    string filename = "auspol2019.csv";

    cout << "Cargando tweets" << endl;
    vector<Tweet> tweets = CSVParser::readTweets(filename);
    cout << "Tweets cargados: " << tweets.size() << endl;

    if (tweets.empty()) {
        cerr << "Error: no hay archivo auspol2019.csv" << endl;
        return;
    }

    // CSV para guardar los tiempos de ejecucion
    ofstream csvOut("resultados_experimentos.csv");
    csvOut << "numero_experimento;dataset;estructura_de_datos;"
           << "cantidad_tweets;tiempo_ejecucion_us\n";

    // CSV para guardar la memoria usada
    ofstream csvMem("resultados_memoria.csv");
    csvMem << "estructura_de_datos;clave;num_elementos;memoria_bytes\n";

    int num_experiments = 20;
    int step = 10000;
    int max_tweets = static_cast<int>(tweets.size());

    // Experimentos para medir los tiempos
    for (int exp = 1; exp <= num_experiments; ++exp) {
        cout << "Corriendo exp " << exp << "/" << num_experiments << endl;

        for (int n = step; n <= max_tweets; n += step) {

            // Chaining - user_id
            {
                HashTableChaining<HashUserID> ht;
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_id);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";Chaining_user_id;" << n << ";" << dur << "\n";
            }

            // Chaining - user_screen_name
            {
                HashTableChaining<HashScreenName> ht;
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_screen_name);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";Chaining_user_screen_name;" << n << ";" << dur << "\n";
            }

            // Closed Linear - user_id
            {
                HashTableClosed<HashUserIDClosed, HashUserIDSecond>
                    ht(400009, ProbingStrategy::LINEAR);
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_id);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";ClosedLinear_user_id;" << n << ";" << dur << "\n";
            }

            // Closed Linear - user_screen_name
            {
                HashTableClosed<HashScreenNameClosed, HashScreenNameSecond>
                    ht(400009, ProbingStrategy::LINEAR);
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_screen_name);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";ClosedLinear_user_screen_name;" << n << ";" << dur << "\n";
            }

            // Closed Quadratic - user_id
            {
                HashTableClosed<HashUserIDClosed, HashUserIDSecond>
                    ht(400009, ProbingStrategy::QUADRATIC);
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_id);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";ClosedQuadratic_user_id;" << n << ";" << dur << "\n";
            }

            // Closed Quadratic - user_screen_name
            {
                HashTableClosed<HashScreenNameClosed, HashScreenNameSecond>
                    ht(400009, ProbingStrategy::QUADRATIC);
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_screen_name);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";ClosedQuadratic_user_screen_name;" << n << ";" << dur << "\n";
            }

            // Closed Double Hashing - user_id
            {
                HashTableClosed<HashUserIDClosed, HashUserIDSecond>
                    ht(400009, ProbingStrategy::DOUBLE_HASHING);
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_id);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";ClosedDoubleHashing_user_id;" << n << ";" << dur << "\n";
            }

            // Closed Double Hashing - user_screen_name
            {
                HashTableClosed<HashScreenNameClosed, HashScreenNameSecond>
                    ht(400009, ProbingStrategy::DOUBLE_HASHING);
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    ht.insert(tweets[i].user_screen_name);
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";ClosedDoubleHashing_user_screen_name;" << n << ";" << dur << "\n";
            }

            // STL Unordered Map - user_id
            {
                unordered_map<string, int> um;
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    um[tweets[i].user_id]++;
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";STLUnorderedMap_user_id;" << n << ";" << dur << "\n";
            }

            // STL Unordered Map - user_screen_name
            {
                unordered_map<string, int> um;
                auto start = high_resolution_clock::now();
                for (int i = 0; i < n; ++i) {
                    um[tweets[i].user_screen_name]++;
                }
                auto stop = high_resolution_clock::now();
                auto dur = duration_cast<microseconds>(stop - start).count();
                csvOut << exp << ";" << dataset
                       << ";STLUnorderedMap_user_screen_name;" << n << ";" << dur << "\n";
            }
        }
    }

    csvOut.close();
    cout << "Tiempos listos" << endl;

    // Medición de memoria con todos los tweets
    cout << "Midiendo memoria" << endl;

    // Chaining
    {
        HashTableChaining<HashUserID> ht;
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_id);
        size_t mem = ht.memoryBytes();
        csvMem << "Chaining;user_id;" << ht.size() << ";" << mem << "\n";
        cout << "Chaining user_id: " << mem / 1024.0 << " KB" << endl;
    }
    {
        HashTableChaining<HashScreenName> ht;
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_screen_name);
        size_t mem = ht.memoryBytes();
        csvMem << "Chaining;user_screen_name;" << ht.size() << ";" << mem << "\n";
        cout << "Chaining user_screen_name: " << mem / 1024.0 << " KB" << endl;
    }

    // Closed Linear
    {
        HashTableClosed<HashUserIDClosed, HashUserIDSecond> ht(400009, ProbingStrategy::LINEAR);
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_id);
        size_t mem = ht.memoryBytes();
        csvMem << "ClosedLinear;user_id;" << ht.size() << ";" << mem << "\n";
        cout << "ClosedLinear user_id: " << mem / 1024.0 << " KB" << endl;
    }
    {
        HashTableClosed<HashScreenNameClosed, HashScreenNameSecond> ht(400009, ProbingStrategy::LINEAR);
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_screen_name);
        size_t mem = ht.memoryBytes();
        csvMem << "ClosedLinear;user_screen_name;" << ht.size() << ";" << mem << "\n";
        cout << "ClosedLinear user_screen_name: " << mem / 1024.0 << " KB" << endl;
    }

    // Closed Quadratic
    {
        HashTableClosed<HashUserIDClosed, HashUserIDSecond> ht(400009, ProbingStrategy::QUADRATIC);
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_id);
        size_t mem = ht.memoryBytes();
        csvMem << "ClosedQuadratic;user_id;" << ht.size() << ";" << mem << "\n";
        cout << "ClosedQuadratic user_id: " << mem / 1024.0 << " KB" << endl;
    }
    {
        HashTableClosed<HashScreenNameClosed, HashScreenNameSecond> ht(400009, ProbingStrategy::QUADRATIC);
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_screen_name);
        size_t mem = ht.memoryBytes();
        csvMem << "ClosedQuadratic;user_screen_name;" << ht.size() << ";" << mem << "\n";
        cout << "ClosedQuadratic user_screen_name: " << mem / 1024.0 << " KB" << endl;
    }

    // Closed Double Hashing
    {
        HashTableClosed<HashUserIDClosed, HashUserIDSecond> ht(400009, ProbingStrategy::DOUBLE_HASHING);
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_id);
        size_t mem = ht.memoryBytes();
        csvMem << "ClosedDoubleHashing;user_id;" << ht.size() << ";" << mem << "\n";
        cout << "ClosedDoubleHashing user_id: " << mem / 1024.0 << " KB" << endl;
    }
    {
        HashTableClosed<HashScreenNameClosed, HashScreenNameSecond> ht(400009, ProbingStrategy::DOUBLE_HASHING);
        for (int i = 0; i < max_tweets; ++i) ht.insert(tweets[i].user_screen_name);
        size_t mem = ht.memoryBytes();
        csvMem << "ClosedDoubleHashing;user_screen_name;" << ht.size() << ";" << mem << "\n";
        cout << "ClosedDoubleHashing user_screen_name: " << mem / 1024.0 << " KB" << endl;
    }

    // STL Unordered Map
    {
        unordered_map<string, int> um;
        for (int i = 0; i < max_tweets; ++i) um[tweets[i].user_id]++;
        size_t mem = estimateUnorderedMapMemory(um);
        csvMem << "STLUnorderedMap;user_id;" << um.size() << ";" << mem << "\n";
        cout << "STLUnorderedMap user_id: " << mem / 1024.0 << " KB" << endl;
    }
    {
        unordered_map<string, int> um;
        for (int i = 0; i < max_tweets; ++i) um[tweets[i].user_screen_name]++;
        size_t mem = estimateUnorderedMapMemory(um);
        csvMem << "STLUnorderedMap;user_screen_name;" << um.size() << ";" << mem << "\n";
        cout << "STLUnorderedMap user_screen_name: " << mem / 1024.0 << " KB" << endl;
    }

    csvMem.close();
    cout << "Listo, todo guardado." << endl;
}

int main() {
    runExperiments();
    return 0;
}
