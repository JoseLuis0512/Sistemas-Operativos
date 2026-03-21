#ifndef CONSUMIDOR_HPP
#define CONSUMIDOR_HPP
#include "Buffer.hpp" // - Incluimos Buffer, el recurso compartido del que el consumidor extrae números
#include <thread>     // - Incluimos thread para ejecutar el consumidor en un hilo separado
#include <atomic>     // - Incluimos atomic para el flag de control seguro entre hilos
#include <string>     // - Incluimos string para el nombre identificador del consumidor

class Consumidor
{
private:
    Buffer *buffer;            // - Puntero al buffer compartido del que se extraen números
    std::thread hilo;          // - Hilo que ejecutará la lógica del consumidor
    std::atomic<bool> running; // - Flag atómico que indica si el consumidor debe seguir ejecutándose
    bool (*predicate)(int);    // - Puntero a función que define qué tipo de número consume este consumidor
    int suma;                  // - Acumulador de la suma de todos los números consumidos
    bool blocked;              // - Indica si el consumidor está bloqueado esperando datos
    std::string nombre;        // - Nombre identificador del consumidor (ej: "Pares", "Impares", "Primos")

    void run(); // - Función principal del consumidor: extrae y acumula números del buffer

public:
    Consumidor(Buffer *buffer, // - Constructor: recibe el buffer, el predicado y el nombre
               bool (*predicate)(int),
               const std::string &nombre);
    ~Consumidor();                 // - Destructor: detiene el hilo del consumidor
    int getSuma() const;           // - Retorna la suma acumulada de los números consumidos
    bool isBlocked() const;        // - Retorna true si el consumidor está bloqueado esperando datos
    std::string getNombre() const; // - Retorna el nombre identificador del consumidor
};
#endif