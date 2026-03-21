#include "PCScene.hpp"            // - Incluimos el header de PCScene con sus declaraciones
#include "../app/Application.hpp" // - Incluimos Application para poder interactuar con la app
#include "raylib.h"               // - Incluimos raylib para funciones gráficas y de tiempo
#include <cstdlib>                // - Incluimos cstdlib para funciones como std::srand
#include <ctime>                  // - Incluimos ctime para obtener la hora actual con std::time
#include <thread>                 // - Incluimos thread para manejo de hilos
#include <mutex>                  // - Incluimos mutex para sincronización entre hilos
#include <condition_variable>     // - Incluimos condition_variable para coordinar hilos
#include <atomic>                 // - Incluimos atomic para el flag de control seguro entre hilos

// --------------------------------------------------
// Constructor
// --------------------------------------------------
PCScene::PCScene(Application *app)
{
    this->app = app; // - Guardamos el puntero a la aplicación en el atributo de instancia

    maxSize = 10;   // - Definimos el tamaño máximo del buffer compartido
    running = true; // - Indicamos que los hilos deben comenzar a ejecutarse

    std::srand(std::time(nullptr)); // - Inicializamos la semilla de rand con la hora actual

    producerThread = std::thread(&PCScene::producer, this);           // - Creamos el hilo del productor
    consumerEvenThread = std::thread(&PCScene::consumerEven, this);   // - Creamos el hilo del consumidor de pares
    consumerOddThread = std::thread(&PCScene::consumerOdd, this);     // - Creamos el hilo del consumidor de impares
    consumerPrimeThread = std::thread(&PCScene::consumerPrime, this); // - Creamos el hilo del consumidor de primos
}

// --------------------------------------------------
// Destructor: detener threads y unirlos
// --------------------------------------------------
PCScene::~PCScene()
{
    running = false; // - Marcamos el flag para indicar que los hilos deben detenerse

    notEmpty.notify_all(); // - Despertamos todos los hilos que esperan en notEmpty para que puedan salir
    notFull.notify_all();  // - Despertamos todos los hilos que esperan en notFull para que puedan salir

    producerThread.join();      // - Esperamos a que el hilo del productor termine completamente
    consumerEvenThread.join();  // - Esperamos a que el hilo del consumidor de pares termine
    consumerOddThread.join();   // - Esperamos a que el hilo del consumidor de impares termine
    consumerPrimeThread.join(); // - Esperamos a que el hilo del consumidor de primos termine
}

// --------------------------------------------------
// Función para verificar si un número es primo
// --------------------------------------------------
bool PCScene::isPrime(int n)
{
    if (n <= 1) // - Los números menores o iguales a 1 no son primos
        return false;
    for (int i = 2; i * i <= n; i++) // - Iteramos desde 2 hasta la raíz cuadrada de n
        if (n % i == 0)              // - Si n es divisible por i, no es primo
            return false;
    return true; // - Si no encontramos divisores, el número es primo
}

// --------------------------------------------------
// Productor
// --------------------------------------------------
void PCScene::producer()
{
    while (running) // - El hilo sigue produciendo mientras running sea verdadero
    {
        std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura

        notFull.wait(lock, [this]() // - Esperamos si el buffer está lleno o si debemos detenernos
                     { return buffer.size() < maxSize || !running; });

        if (!running) // - Si se indicó detener, salimos del bucle
            break;

        int num = GetRandomValue(1, 100); // - Generamos un número aleatorio entre 1 y 100
        buffer.push_back(num);            // - Insertamos el número al final del buffer

        lock.unlock();         // - Liberamos el mutex antes de notificar
        notEmpty.notify_all(); // - Notificamos a los consumidores que hay datos disponibles

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // - Pausamos 500 ms antes de producir de nuevo
    }
}

// --------------------------------------------------
// Consumidor de números pares (no primos)
// --------------------------------------------------
void PCScene::consumerEven()
{
    while (running) // - El hilo sigue consumiendo mientras running sea verdadero
    {
        std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura

        notEmpty.wait(lock, [this]() // - Esperamos si el buffer está vacío o si debemos detenernos
                      { return !buffer.empty() || !running; });

        if (!running) // - Si se indicó detener, salimos del bucle
            break;

        for (auto it = buffer.begin(); it != buffer.end(); ++it) // - Recorremos el buffer en busca del objetivo
        {
            if (*it % 2 == 0 && !isPrime(*it)) // - Buscamos un número par que no sea primo
            {
                sumEven += *it;   // - Acumulamos el valor en la suma de pares
                buffer.erase(it); // - Eliminamos el elemento del buffer
                break;            // - Salimos del for tras consumir un elemento
            }
        }

        lock.unlock();        // - Liberamos el mutex antes de notificar
        notFull.notify_all(); // - Notificamos al productor que hay espacio disponible

        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // - Pausamos 700 ms antes de consumir de nuevo
    }
}

// --------------------------------------------------
// Consumidor de números impares (no primos)
// --------------------------------------------------
void PCScene::consumerOdd()
{
    while (running) // - El hilo sigue consumiendo mientras running sea verdadero
    {
        std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura

        notEmpty.wait(lock, [this]() // - Esperamos si el buffer está vacío o si debemos detenernos
                      { return !buffer.empty() || !running; });

        if (!running) // - Si se indicó detener, salimos del bucle
            break;

        for (auto it = buffer.begin(); it != buffer.end(); ++it) // - Recorremos el buffer en busca del objetivo
        {
            if (*it % 2 != 0 && !isPrime(*it)) // - Buscamos un número impar que no sea primo
            {
                sumOdd += *it;    // - Acumulamos el valor en la suma de impares
                buffer.erase(it); // - Eliminamos el elemento del buffer
                break;            // - Salimos del for tras consumir un elemento
            }
        }

        lock.unlock();        // - Liberamos el mutex antes de notificar
        notFull.notify_all(); // - Notificamos al productor que hay espacio disponible

        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // - Pausamos 700 ms antes de consumir de nuevo
    }
}

// --------------------------------------------------
// Consumidor de números primos
// --------------------------------------------------
void PCScene::consumerPrime()
{
    while (running) // - El hilo sigue consumiendo mientras running sea verdadero
    {
        std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para acceder al buffer de forma segura

        notEmpty.wait(lock, [this]() // - Esperamos si el buffer está vacío o si debemos detenernos
                      { return !buffer.empty() || !running; });

        if (!running) // - Si se indicó detener, salimos del bucle
            break;

        for (auto it = buffer.begin(); it != buffer.end(); ++it) // - Recorremos el buffer en busca del objetivo
        {
            if (isPrime(*it)) // - Buscamos un número primo
            {
                sumPrime += *it;  // - Acumulamos el valor en la suma de primos
                buffer.erase(it); // - Eliminamos el elemento del buffer
                break;            // - Salimos del for tras consumir un elemento
            }
        }

        lock.unlock();        // - Liberamos el mutex antes de notificar
        notFull.notify_all(); // - Notificamos al productor que hay espacio disponible

        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // - Pausamos 700 ms antes de consumir de nuevo
    }
}

// --------------------------------------------------
// Update (solo animación, ya no toca buffer)
// --------------------------------------------------
void PCScene::update()
{
    timer += GetFrameTime(); // - Acumulamos el tiempo transcurrido desde el último frame
}

// --------------------------------------------------
// Draw: dibuja el buffer, los números y sumas
// --------------------------------------------------
void PCScene::draw()
{
    DrawText("PRODUCTOR - CONSUMIDOR", 250, 50, 30, BLACK); // - Dibujamos el título de la escena

    int startX = 100; // - Posición X inicial donde comenzamos a dibujar las celdas del buffer
    int startY = 200; // - Posición Y inicial donde comenzamos a dibujar las celdas del buffer
    int boxSize = 50; // - Tamaño en píxeles de cada celda del buffer

    std::unique_lock<std::mutex> lock(mtx); // - Adquirimos el mutex para leer el buffer de forma segura

    for (int i = 0; i < maxSize; i++) // - Iteramos por cada posición posible del buffer
    {
        Rectangle box = {
            // - Definimos el rectángulo de la celda actual
            (float)(startX + i * (boxSize + 10)), // - Posición X con espaciado entre celdas
            (float)startY,                        // - Posición Y fija para todas las celdas
            (float)boxSize,                       // - Ancho de la celda
            (float)boxSize                        // - Alto de la celda
        };

        if (i < (int)buffer.size()) // - Si hay un elemento en esta posición, lo dibujamos con color
        {
            int val = buffer[i]; // - Obtenemos el valor del elemento actual
            Color color = GREEN; // - Color por defecto (no debería usarse, solo como fallback)

            if (isPrime(val)) // - Si es primo, usamos rojo
                color = RED;
            else if (val % 2 == 0) // - Si es par (y no primo), usamos azul
                color = BLUE;
            else // - Si es impar (y no primo), usamos naranja
                color = ORANGE;

            DrawRectangleRec(box, color);                                       // - Dibujamos la celda con el color asignado
            DrawText(TextFormat("%d", val), box.x + 10, box.y + 15, 20, BLACK); // - Dibujamos el número dentro de la celda
        }
        else
        {
            DrawRectangleLinesEx(box, 2, GRAY); // - Si la celda está vacía, dibujamos solo su contorno
        }
    }

    lock.unlock(); // - Liberamos el mutex al terminar de leer el buffer

    DrawText(TextFormat("Elementos: %d / %d", (int)buffer.size(), maxSize), 300, 300, 20, BLACK); // - Mostramos ocupación actual del buffer
    DrawText(TextFormat("Suma pares: %d", sumEven), 100, 400, 20, BLUE);                          // - Mostramos la suma acumulada de pares
    DrawText(TextFormat("Suma impares: %d", sumOdd), 100, 430, 20, ORANGE);                       // - Mostramos la suma acumulada de impares
    DrawText(TextFormat("Suma primos: %d", sumPrime), 100, 460, 20, RED);                         // - Mostramos la suma acumulada de primos
}