#include "Buffer.hpp" // - Incluimos el header de Buffer con sus declaraciones

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Buffer::Buffer(int maxSize)
{
    this->maxSize = maxSize; // - Guardamos el tamaño máximo del buffer
}

// --------------------------------------------------
// Insertar número (productor)
// --------------------------------------------------
void Buffer::push(int value)
{
    std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura

    notFull.wait(lock, [this]() // - Esperamos si el buffer está lleno
                 { return (int)data.size() < maxSize; });

    data.push_back(value); // - Insertamos el número al final del buffer

    notEmpty.notify_all(); // - Notificamos a los consumidores que hay un nuevo elemento
}

// --------------------------------------------------
// Extraer número que cumpla un predicado (consumidor)
// --------------------------------------------------
bool Buffer::popIf(int &out, bool (*predicate)(int))
{
    std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura

    notEmpty.wait(lock, [this]() // - Esperamos si el buffer está vacío
                  { return !data.empty(); });

    for (auto it = data.begin(); it != data.end(); ++it) // - Recorremos el buffer buscando un elemento válido
    {
        if (predicate(*it)) // - Verificamos si el elemento cumple la condición del consumidor
        {
            out = *it;            // - Guardamos el valor extraído en la variable de salida
            data.erase(it);       // - Eliminamos el elemento del buffer
            notFull.notify_all(); // - Notificamos al productor que hay espacio disponible
            return true;          // - Indicamos que se extrajo un elemento exitosamente
        }
    }

    return false; // - No se encontró ningún elemento que cumpla el predicado en este intento
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