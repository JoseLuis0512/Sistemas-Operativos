#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <vector>             // - Incluimos vector para almacenar los elementos del buffer
#include <mutex>              // - Incluimos mutex para proteger el acceso concurrente al buffer
#include <condition_variable> // - Incluimos condition_variable para coordinar productor y consumidores

class Buffer
{
private:
    std::vector<int> data;            // - Vector que almacena los números producidos y aún no consumidos
    int maxSize;                      // - Capacidad máxima del buffer
    std::mutex mtx;                   // - Mutex que protege el acceso al buffer
    std::condition_variable notFull;  // - Señal para despertar al productor cuando hay espacio
    std::condition_variable notEmpty; // - Señal para despertar a los consumidores cuando hay datos

public:
    Buffer(int maxSize);  // - Constructor: inicializa el buffer con un tamaño máximo
    void push(int value); // - Inserta un número en el buffer; bloquea si está lleno
    bool popIf(int &out,  // - Extrae un número que cumpla el predicado; retorna false si no hay ninguno
               bool (*predicate)(int));
    int getCurrentSize();        // - Retorna la cantidad actual de elementos en el buffer
    int getMaxSize();            // - Retorna el tamaño máximo del buffer
    std::vector<int> snapshot(); // - Retorna una copia del buffer para dibujarlo sin bloquear mucho tiempo
};
#endif