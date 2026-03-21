#include "Productor.hpp" // - Incluimos el header del Productor con sus declaraciones
#include <fstream>       // - Incluimos fstream para leer el archivo de números
#include <sstream>       // - Incluimos sstream para parsear la línea separada por comas
#include <string>        // - Incluimos string para manejar tokens del archivo
#include <thread>        // - Incluimos thread para pausas con sleep_for
#include <chrono>        // - Incluimos chrono para definir duraciones de tiempo

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Productor::Productor(Buffer *buffer, const std::string &filePath)
{
    this->buffer = buffer;     // - Guardamos el puntero al buffer compartido
    this->filePath = filePath; // - Guardamos la ruta del archivo de entrada
    this->running = true;      // - Marcamos el hilo como activo
    this->blocked = false;     // - Inicialmente el productor no está bloqueado

    hilo = std::thread(&Productor::run, this); // - Creamos el hilo que ejecutará la función run
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
Productor::~Productor()
{
    running = false; // - Indicamos al hilo que debe detenerse
    hilo.join();     // - Esperamos a que el hilo termine completamente
}

// --------------------------------------------------
// Lógica principal del productor
// --------------------------------------------------
void Productor::run()
{
    std::ifstream file(filePath); // - Abrimos el archivo de números en modo lectura

    if (!file.is_open()) // - Verificamos que el archivo se haya abierto correctamente
        return;          // - Si no se pudo abrir, terminamos el hilo sin hacer nada

    std::string linea;         // - Variable para leer cada línea del archivo
    std::getline(file, linea); // - Leemos toda la línea (los números están en una sola línea separados por comas)

    std::stringstream ss(linea); // - Creamos un stream a partir de la línea para parsearla
    std::string token;           // - Variable temporal para cada token entre comas

    while (running && std::getline(ss, token, ',')) // - Extraemos tokens separados por coma mientras el hilo esté activo
    {
        if (token.empty()) // - Si el token está vacío (coma extra), lo ignoramos
            continue;

        int num = std::stoi(token); // - Convertimos el token a entero

        blocked = true;    // - Marcamos que el productor puede bloquearse al intentar insertar
        buffer->push(num); // - Insertamos el número en el buffer (bloquea si está lleno)
        blocked = false;   // - El productor ya insertó, ya no está bloqueado

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // - Esperamos 500 ms antes de producir el siguiente
    }

    running = false; // - Al terminar el archivo, marcamos el hilo como inactivo
}

// --------------------------------------------------
// Getter de estado
// --------------------------------------------------
bool Productor::isBlocked() const
{
    return blocked; // - Retornamos si el productor está esperando espacio en el buffer
}