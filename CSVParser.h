#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Tweet.h"

/**
 * @brief Clase para extraer informacion del dataset.
 * Permite parsear los archivos CSV separando los campos adecuadamente
 * (incluso si tienen comas dentro de campos entre comillas).
 */
class CSVParser {
private:
    // Funcion de ayuda para separar una línea respetando las comillas
    static std::vector<std::string> parseCSVLine(const std::string& line) {
        std::vector<std::string> result;
        std::string current = "";
        bool inQuotes = false;

        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            if (c == '\"') {
                inQuotes = !inQuotes; // Cambia estado si entra o sale de comillas
            } else if (c == ',' && !inQuotes) {
                result.push_back(current);
                current = "";
            } else {
                current += c;
            }
        }
        result.push_back(current);
        return result;
    }

public:
    static std::vector<Tweet> readTweets(const std::string& filename) {
        std::vector<Tweet> tweets;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error" << filename << "\n";
            return tweets;
        }

        std::string line;
        std::getline(file, line);

        std::string fullRow = "";
        bool inQuotes = false;

        while (std::getline(file, line)) {
            if (fullRow.empty()) {
                fullRow = line;
            } else {
                fullRow += "\n" + line; // Concatena si el tweet tiene salto de linea
            }

            // Contar comillas para saber si la fila CSV termino realmente
            for (char c : line) {
                if (c == '\"') inQuotes = !inQuotes;
            }

            // Si no esta dentro de comillas abiertas, se procesa la fila completa
            if (!inQuotes) {
                std::vector<std::string> cols = parseCSVLine(fullRow);

                // Se asegura que la fila tenga al menos 8 columnas (user_screen_name es la col 7)
                if (cols.size() >= 8) {
                    Tweet t;
                    t.user_id = cols[5];
                    t.user_screen_name = cols[7];
                    tweets.push_back(t);
                }
                fullRow = ""; // Reinicia para la siguiente fila
            }
        }
        file.close();
        return tweets;
    }
};