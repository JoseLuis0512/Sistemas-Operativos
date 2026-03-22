#ifndef MESA_HPP // - Evitamos que este archivo se incluya más de una vez
#define MESA_HPP // - Definimos el macro que indica que ya fue incluido

#include "Filosfo.hpp" // - Incluimos Filósofo para manejar los hilos de cada comensal
#include "Tenedor.hpp" // - Incluimos Tenedor para manejar los recursos compartidos
#include <vector>      // - Incluimos vector para listas dinámicas de filósofos y tenedores
#include <memory>      // - Incluimos memory para usar unique_ptr y manejar memoria automáticamente
#include <string>      // - Incluimos string para los nombres de los filósofos

class Mesa
{
private:
    int n; // - Número de filósofos (y tenedores) en la mesa

    std::vector<std::unique_ptr<Tenedor>> tenedores;  // - Lista de tenedores en la mesa
    std::vector<std::unique_ptr<Filosofo>> filosofos; // - Lista de filósofos en la mesa

public:
    Mesa(int n); // - Constructor: crea n filósofos y n tenedores y lanza los hilos

    int getN() const; // - Retorna el número de filósofos en la mesa

    // - Retorna referencia a la lista de tenedores para que la escena los dibuje
    const std::vector<std::unique_ptr<Tenedor>> &getTenedores() const;

    // - Retorna referencia a la lista de filósofos para que la escena los dibuje
    const std::vector<std::unique_ptr<Filosofo>> &getFilosofos() const;
};

#endif // - Fin del bloque condicional que evita doble inclusión