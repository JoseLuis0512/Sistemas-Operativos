#ifndef BUFFER_HPP
#define BUFFER_HPP
#include <vector>             // - Incluimos vector para almacenar los elementos del buffer
#include <mutex>              // - Incluimos mutex para proteger el acceso concurrente al buffer
#include <condition_variable> // - Incluimos condition_variable para coordinar productor y consumidores
#include <atomic>             // - Incluimos atomic para los flags de evento visibles desde el hilo de render

class Buffer
{
private:
    std::vector<int> data;            // - Vector que almacena los números producidos y aún no consumidos
    int maxSize;                      // - Capacidad máxima del buffer
    std::mutex mtx;                   // - Mutex que protege el acceso al buffer
    std::condition_variable notFull;  // - Señal para despertar al productor cuando hay espacio
    std::condition_variable notEmpty; // - Señal para despertar a los consumidores cuando hay datos

public:
    // - Flags atómicos de eventos: seguros para leer desde el hilo de render sin tomar el mutex
    std::atomic<int> lastInsertIndex;  // - Índice donde se insertó el último elemento (-1 = sin evento)
    std::atomic<int> lastRemoveIndex;  // - Índice desde donde se extrajo el último elemento (-1 = sin evento)
    std::atomic<bool> isFull;          // - true cuando el buffer está completamente lleno
    std::atomic<bool> isEmpty;         // - true cuando el buffer está completamente vacío
    std::atomic<bool> criticalSection; // - true mientras un hilo tiene el mutex y opera sobre el buffer
    std::atomic<bool> active;          // - false cuando se llama shutdown(), despierta hilos bloqueados

    Buffer(int maxSize);                          // - Constructor: inicializa el buffer con un tamaño máximo
    void shutdown();                              // - Despierta todos los hilos bloqueados para que puedan terminar
    void push(int value);                         // - Inserta un número en el buffer; bloquea si está lleno
    bool popIf(int &out, bool (*predicate)(int)); // - Extrae un número que cumpla el predicado
    int getCurrentSize();                         // - Retorna la cantidad actual de elementos en el buffer
    int getMaxSize();                             // - Retorna el tamaño máximo del buffer
    std::vector<int> snapshot();                  // - Retorna una copia del buffer para dibujarlo de forma segura
};
#endif