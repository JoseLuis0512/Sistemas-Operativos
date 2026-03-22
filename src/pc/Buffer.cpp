#include "Buffer.hpp" // - Incluimos el header de Buffer con sus declaraciones

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Buffer::Buffer(int maxSize)
    : maxSize(maxSize),      // - Guardamos el tamaño máximo del buffer
      lastInsertIndex(-1),   // - Sin evento de inserción pendiente al inicio
      lastRemoveIndex(-1),   // - Sin evento de extracción pendiente al inicio
      isFull(false),         // - El buffer inicia vacío, no lleno
      isEmpty(true),         // - El buffer inicia completamente vacío
      criticalSection(false) // - No hay sección crítica activa al inicio
{
}

// --------------------------------------------------
// Insertar número (productor)
// --------------------------------------------------
void Buffer::push(int value)
{
    std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura
    criticalSection = true;                 // - Marcamos que la sección crítica está activa

    notFull.wait(lock, [this]() // - Esperamos si el buffer está lleno
                 { return (int)data.size() < maxSize; });

    data.push_back(value);                  // - Insertamos el número al final del buffer
    lastInsertIndex = (int)data.size() - 1; // - Registramos el índice de inserción para la animación
    isFull = ((int)data.size() == maxSize); // - Actualizamos el flag de buffer lleno
    isEmpty = false;                        // - El buffer ya no está vacío

    criticalSection = false; // - La sección crítica termina
    notEmpty.notify_all();   // - Notificamos a los consumidores que hay un nuevo elemento
}

// --------------------------------------------------
// Extraer número que cumpla un predicado (consumidor)
// --------------------------------------------------
bool Buffer::popIf(int &out, bool (*predicate)(int))
{
    std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura
    criticalSection = true;                 // - Marcamos que la sección crítica está activa

    notEmpty.wait(lock, [this]() // - Esperamos si el buffer está vacío
                  { return !data.empty(); });

    for (auto it = data.begin(); it != data.end(); ++it) // - Recorremos el buffer buscando un elemento válido
    {
        if (predicate(*it)) // - Verificamos si el elemento cumple la condición del consumidor
        {
            out = *it;                                  // - Guardamos el valor extraído
            lastRemoveIndex = (int)(it - data.begin()); // - Registramos el índice de extracción para la animación
            data.erase(it);                             // - Eliminamos el elemento del buffer
            isFull = false;                             // - El buffer ya no puede estar lleno
            isEmpty = data.empty();                     // - Actualizamos el flag de buffer vacío

            criticalSection = false; // - La sección crítica termina
            notFull.notify_all();    // - Notificamos al productor que hay espacio disponible
            return true;             // - Indicamos que se extrajo un elemento exitosamente
        }
    }

    criticalSection = false; // - La sección crítica termina aunque no se encontró elemento
    return false;            // - No se encontró ningún elemento que cumpla el predicado
}

// --------------------------------------------------
// Getters
// --------------------------------------------------
int Buffer::getCurrentSize()
{
    std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para leer el tamaño de forma segura
    return (int)data.size();                // - Retornamos la cantidad actual de elementos
}

int Buffer::getMaxSize()
{
    return maxSize; // - Retornamos la capacidad máxima del buffer
}

std::vector<int> Buffer::snapshot()
{
    std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para hacer la copia de forma segura
    return data;                            // - Retornamos una copia del vector para uso en el draw
}