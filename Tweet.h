#pragma once
#include <string>

/**
 * @brief Estructura que representa un Tweet.
 * Contiene la informacion basica extraida del CSV:
 * el ID del usuario y su nombre de pantalla (screen name).
 */
struct Tweet {
    std::string user_id;
    std::string user_screen_name;
};