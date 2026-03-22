#include "Mesa.hpp" // - Incluimos el header de Mesa con sus declaraciones

// - Nombres clásicos de los filósofos comensales de Dijkstra
static const std::string nombres[] = {
    "Aristoteles", // - Filósofo 0
    "Platon",      // - Filósofo 1
    "Socrates",    // - Filósofo 2
    "Confucio",    // - Filósofo 3
    "Descartes",   // - Filósofo 4
    "Kant",        // - Filósofo 5
    "Nietzsche",   // - Filósofo 6
    "Hume",        // - Filósofo 7
    "Locke",       // - Filósofo 8
    "Spinoza"      // - Filósofo 9
};

// --------------------------------------------------
// Constructor: crea tenedores y filósofos y lanza los hilos
// --------------------------------------------------
Mesa::Mesa(int n) : n(n)
{
    // - Creamos los n tenedores primero (los filósofos los necesitan)
    for (int i = 0; i < n; i++)
        tenedores.push_back(std::make_unique<Tenedor>(i)); // - Tenedor i entre filósofo i y (i+1)%n

    // - Creamos los n filósofos asignando sus tenedores izquierdo y derecho
    for (int i = 0; i < n; i++)
    {
        Tenedor *izq = tenedores[i].get();           // - Tenedor izquierdo del filósofo i
        Tenedor *der = tenedores[(i + 1) % n].get(); // - Tenedor derecho (circular)
        std::string nombre = nombres[i];             // - Nombre del filósofo

        filosofos.push_back(std::make_unique<Filosofo>(i, nombre, izq, der)); // - Creamos el filósofo
    }
}

// --------------------------------------------------
// Getters
// --------------------------------------------------
int Mesa::getN() const
{
    return n; // - Retornamos el número de filósofos
}

const std::vector<std::unique_ptr<Tenedor>> &Mesa::getTenedores() const
{
    return tenedores; // - Retornamos la lista de tenedores
}

const std::vector<std::unique_ptr<Filosofo>> &Mesa::getFilosofos() const
{
    return filosofos; // - Retornamos la lista de filósofos
}