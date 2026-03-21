#ifndef PRODUCTOR_HPP
#define PRODUCTOR_HPP
#include "Buffer.hpp" // - Incluimos Buffer, el recurso compartido donde el productor depositará números
#include <thread>     // - Incluimos thread para ejecutar el productor en un hilo separado
#include <atomic>     // - Incluimos atomic para el flag de control seguro entre hilos
#include <string>     // - Incluimos string para recibir la ruta del archivo de entrada

class Productor
{
private:
    Buffer *buffer;            // - Puntero al buffer compartido donde se depositan los números
    std::thread hilo;          // - Hilo que ejecutará la lógica del productor
    std::atomic<bool> running; // - Flag atómico que indica si el productor debe seguir ejecutándose
    std::string filePath;      // - Ruta del archivo del que se leerán los números
    bool blocked;              // - Indica si el productor está bloqueado esperando espacio en el buffer

    void run(); // - Función principal del productor: lee el archivo e inserta números en el buffer

public:
    Productor(Buffer *buffer, const std::string &filePath); // - Constructor: recibe el buffer y la ruta del archivo
    ~Productor();                                           // - Destructor: detiene el hilo del productor
    bool isBlocked() const;                                 // - Retorna true si el productor está bloqueado esperando espacio
};
#endif