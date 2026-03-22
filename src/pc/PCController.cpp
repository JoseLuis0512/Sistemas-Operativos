#include "PCController.hpp" // - Incluimos el header del controlador con sus declaraciones

// --------------------------------------------------
// Funciones predicado globales
// --------------------------------------------------
bool esPrimo(int n) // - Determina si n es primo; esta condición tiene prioridad sobre par/impar
{
    if (n <= 1)
        return false; // - Los números menores o iguales a 1 no son primos
    if (n <= 3)
        return true;              // - 2 y 3 son primos directamente
    if (n % 2 == 0 || n % 3 == 0) // - Múltiplos de 2 y 3 no son primos
        return false;
    for (int i = 5; i * i <= n; i += 6) // - Revisamos divisores de la forma 6k±1
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true; // - Si no tiene divisores, es primo
}

bool esPar(int n) // - Retorna true solo si n es par Y no es primo (2 es primo, no cuenta aquí)
{
    return n % 2 == 0 && !esPrimo(n); // - Condición: par y no primo
}

bool esImpar(int n) // - Retorna true solo si n es impar Y no es primo
{
    return n % 2 != 0 && !esPrimo(n); // - Condición: impar y no primo
}

// --------------------------------------------------
// Constructor: solo inicializa el buffer, sin hilos aún
// --------------------------------------------------
PCController::PCController(int bufferSize)
    : buffer(bufferSize) // - Inicializamos el buffer con el tamaño recibido
{
    // - Los consumidores se crean en cargarArchivo() para no lanzar hilos en la pantalla de carga
}

// --------------------------------------------------
// Inicia los tres consumidores estándar
// --------------------------------------------------
void PCController::iniciarConsumidores()
{
    consumidores.clear(); // - Limpiamos cualquier consumidor previo

    // - Creamos los tres consumidores con sus respectivos predicados y nombres
    consumidores.push_back(std::make_unique<Consumidor>(&buffer, esPar, "Pares"));     // - Consumidor de pares
    consumidores.push_back(std::make_unique<Consumidor>(&buffer, esImpar, "Impares")); // - Consumidor de impares
    consumidores.push_back(std::make_unique<Consumidor>(&buffer, esPrimo, "Primos"));  // - Consumidor de primos
}

// --------------------------------------------------
// Carga o recarga un archivo, inicia consumidores si es la primera vez, e inicia el productor
// --------------------------------------------------
void PCController::cargarArchivo(const std::string &filePath)
{
    buffer.shutdown(); // - Despertamos hilos previos si los hay antes de destruirlos
    productor.reset(); // - Destruimos el productor anterior si existía

    if (consumidores.empty())  // - Solo creamos los consumidores la primera vez
        iniciarConsumidores(); // - Creamos los tres consumidores con sus predicados

    // - Reactivamos el buffer para la nueva ejecución
    buffer.active = true;                                       // - Volvemos a marcar el buffer como activo
    productor = std::make_unique<Productor>(&buffer, filePath); // - Creamos el productor con el archivo
}

// --------------------------------------------------
// Indica si ya se cargó un archivo
// --------------------------------------------------
bool PCController::tieneArchivo() const
{
    return productor != nullptr; // - Retornamos true si hay un productor activo
}

// --------------------------------------------------
// Getters
// --------------------------------------------------
Buffer *PCController::getBuffer()
{
    return &buffer; // - Retornamos puntero al buffer para que la escena lo consulte
}

Productor *PCController::getProductor()
{
    return productor.get(); // - Retornamos puntero crudo al productor (puede ser nullptr)
}

std::vector<std::unique_ptr<Consumidor>> &PCController::getConsumidores()
{
    return consumidores; // - Retornamos referencia a la lista de consumidores
}