#include <iostream>
#include "HashTabClosed.h"

using namespace std;

int main() {
    cout << "Prueba Hashing Cerrado\n\n";

    // Linear Probing con user_id
    HashTableClosed<HashUserIDClosed, HashUserIDSecond> tablaLinear(
        400009,
        ProbingStrategy::LINEAR
    );

    tablaLinear.insert("123");
    tablaLinear.insert("123");
    tablaLinear.insert("456");

    cout << "Linear Probing - user_id\n";
    cout << "Conteo 123: " << tablaLinear.get("123") << endl;
    cout << "Conteo 456: " << tablaLinear.get("456") << endl;
    cout << "Claves distintas: " << tablaLinear.size() << endl;
    cout << "Colisiones: " << tablaLinear.getCollisions() << endl;
    cout << endl;

    // Quadratic Probing con user_id
    HashTableClosed<HashUserIDClosed, HashUserIDSecond> tablaQuadratic(
        400009,
        ProbingStrategy::QUADRATIC
    );

    tablaQuadratic.insert("123");
    tablaQuadratic.insert("123");
    tablaQuadratic.insert("456");

    cout << "Quadratic Probing - user_id\n";
    cout << "Conteo 123: " << tablaQuadratic.get("123") << endl;
    cout << "Conteo 456: " << tablaQuadratic.get("456") << endl;
    cout << "Claves distintas: " << tablaQuadratic.size() << endl;
    cout << "Colisiones: " << tablaQuadratic.getCollisions() << endl;
    cout << endl;

    // Double Hashing con user_screen_name
    HashTableClosed<HashScreenNameClosed, HashScreenNameSecond> tablaDouble(
        400009,
        ProbingStrategy::DOUBLE_HASHING
    );

    tablaDouble.insert("matias");
    tablaDouble.insert("matias");
    tablaDouble.insert("usuario2");

    cout << "Double Hashing - user_screen_name\n";
    cout << "Conteo matias: " << tablaDouble.get("matias") << endl;
    cout << "Conteo usuario2: " << tablaDouble.get("usuario2") << endl;
    cout << "Claves distintas: " << tablaDouble.size() << endl;
    cout << "Colisiones: " << tablaDouble.getCollisions() << endl;

    return 0;
}