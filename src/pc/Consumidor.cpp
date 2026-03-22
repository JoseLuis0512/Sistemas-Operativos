#include "Consumidor.hpp" // - Incluimos el header del Consumidor con sus declaraciones
#include <thread>         // - Incluimos thread para pausas con sleep_for
#include <chrono>         // - Incluimos chrono para definir duraciones de tiempo

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Consumidor::Consumidor(Buffer *buffer, bool (*predicate)(int), const std::string &nombre)
{
    this->buffer = buffer;       // - Guardamos el puntero al buffer compartido
    this->predicate = predicate; // - Guardamos el predicado que define qué números consume
    this->nombre = nombre;       // - Guardamos el nombre identificador del consumidor
    this->suma = 0;              // - Inicializamos la suma acumulada en cero
    this->running = true;        // - Marcamos el hilo como activo
    this->blocked = false;       // - Inicialmente el consumidor no está bloqueado

    hilo = std::thread(&Consumidor::run, this); // - Creamos el hilo que ejecutará la función run
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
Consumidor::~Consumidor()
{
    running = false; // - Indicamos al hilo que debe detenerse
    hilo.join();     // - Esperamos a que el hilo termine completamente
}

// --------------------------------------------------
// Lógica principal del consumidor
// --------------------------------------------------
void Consumidor::run()
{
    while (running) // - El consumidor sigue extrayendo mientras esté activo
    {
        int valor = 0; // - Variable donde se almacenará el número extraído del buffer

        blocked = true;                               // - Marcamos que el consumidor puede bloquearse al buscar
        bool found = buffer->popIf(valor, predicate); // - Intentamos extraer un número que cumpla el predicado
        blocked = false;                              // - Ya no está bloqueado tras el intento

        if (!buffer->active)
            break; // - Si el buffer fue apagado, salimos del loop inmediatamente

        if (found)         // - Si se extrajo un número válido
            suma += valor; // - Lo acumulamos en la suma del consumidor

        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // - Esperamos 700 ms antes del siguiente intento
    }
}

// --------------------------------------------------
// Getters
// --------------------------------------------------
int Consumidor::getSuma() const
{
    return suma; // - Retornamos la suma acumulada de los números consumidos
}

bool Consumidor::isBlocked() const
{
    return blocked; // - Retornamos si el consumidor está esperando datos en el buffer
}

std::string Consumidor::getNombre() const
{
    return nombre; // - Retornamos el nombre identificador del consumidor
}