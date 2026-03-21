#ifndef PC_SCENE_HPP // - Evitamos que este archivo se incluya más de una vez
#define PC_SCENE_HPP // - Definimos el macro que indica que ya fue incluido

#include "Scene.hpp"          // - Incluimos la clase base Scene de la que PCScene heredará
#include <vector>             // - Incluimos std::vector para manejar arreglos dinámicos
#include <thread>             // - Permite crear y manejar hilos (threads)
#include <mutex>              // - Permite usar mutex para sincronización entre hilos
#include <condition_variable> // - Permite usar variables de condición para coordinación de hilos
#include <atomic>             // - Permite usar variables atómicas, útiles para flags de control seguro en hilos

class Application; // - Forward declaration: le decimos al compilador que existirá la clase Application

class PCScene : public Scene // - Definimos la clase PCScene que hereda de Scene
{
private:              // - Área privada: variables y métodos que solo puede usar esta clase
    Application *app; // - Puntero a la aplicación, permite que la escena interactúe con ella

    std::vector<int> buffer; // - Vector que actúa como buffer compartido entre productor y consumidores
    int maxSize;             // - Tamaño máximo permitido para el buffer

    std::mutex mtx;                   // - Mutex para proteger el acceso al buffer entre hilos
    std::condition_variable notFull;  // - Variable de condición para esperar cuando el buffer está lleno
    std::condition_variable notEmpty; // - Variable de condición para esperar cuando el buffer está vacío

    std::thread producerThread;      // - Hilo que ejecutará la función del productor
    std::thread consumerEvenThread;  // - Hilo que ejecutará la función del consumidor de números pares
    std::thread consumerOddThread;   // - Hilo que ejecutará la función del consumidor de números impares
    std::thread consumerPrimeThread; // - Hilo que ejecutará la función del consumidor de números primos

    std::atomic<bool> running; // - Flag atómico que indica si los hilos deben seguir ejecutándose

    int sumEven = 0;    // - Suma de los números pares procesados
    int sumOdd = 0;     // - Suma de los números impares procesados
    int sumPrime = 0;   // - Suma de los números primos procesados
    float timer = 0.0f; // - Acumula el tiempo transcurrido frame a frame para animaciones

    void producer();      // - Función que genera números y los coloca en el buffer
    void consumerEven();  // - Función que consume números pares del buffer
    void consumerOdd();   // - Función que consume números impares del buffer
    void consumerPrime(); // - Función que consume números primos del buffer

    bool isPrime(int n); // - Función auxiliar que determina si un número es primo

public:                        // - Área pública: métodos accesibles desde fuera de la clase
    PCScene(Application *app); // - Constructor: recibe un puntero a la aplicación
    ~PCScene();                // - Destructor: se encarga de limpiar recursos (hilos, memoria, etc.)

    void update() override; // - Método obligatorio de Scene: ejecuta la lógica de la escena
    void draw() override;   // - Método obligatorio de Scene: renderiza la escena
};

#endif // - Fin del bloque condicional que evita doble inclusión