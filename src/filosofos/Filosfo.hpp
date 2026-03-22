#ifndef FILOSOFO_HPP // - Evitamos que este archivo se incluya más de una vez
#define FILOSOFO_HPP // - Definimos el macro que indica que ya fue incluido

#include "Tenedor.hpp" // - Incluimos Tenedor, los recursos que el filósofo intentará adquirir
#include <thread>      // - Incluimos thread para modelar al filósofo como un hilo
#include <atomic>      // - Incluimos atomic para los flags de estado visibles desde el hilo de render
#include <string>      // - Incluimos string para el nombre del filósofo

// - Estados posibles de un filósofo
enum class EstadoFilosofo
{
    PENSANDO,  // - El filósofo está pensando (no necesita recursos)
    ESPERANDO, // - El filósofo intenta adquirir los tenedores
    COMIENDO   // - El filósofo tiene ambos tenedores y está comiendo
};

class Filosofo
{
private:
    int id;              // - Identificador único del filósofo
    std::string nombre;  // - Nombre del filósofo para mostrar en pantalla
    Tenedor *tenedorIzq; // - Puntero al tenedor de su izquierda
    Tenedor *tenedorDer; // - Puntero al tenedor de su derecha
    std::thread hilo;    // - Hilo que ejecuta el ciclo de vida del filósofo
    int vecesComio;      // - Contador de veces que el filósofo ha comido

    void run(); // - Ciclo de vida: pensar → esperar → comer → repetir

public:
    std::atomic<bool> running;              // - Flag que indica si el hilo debe seguir ejecutándose
    std::atomic<EstadoFilosofo> estado;     // - Estado actual visible desde el hilo de render
    std::atomic<bool> intentando;           // - true mientras intenta adquirir un tenedor
    std::atomic<bool> bloqueado;            // - true si lleva varios intentos fallidos
    std::atomic<int> intentosFallidos;      // - Intentos fallidos consecutivos (para detectar bloqueo)
    std::atomic<int> totalIntentosFallidos; // - Total acumulado de intentos fallidos para resultados

    Filosofo(int id, const std::string &nombre, // - Constructor: recibe ID, nombre y sus dos tenedores
             Tenedor *izq, Tenedor *der);
    ~Filosofo(); // - Destructor: detiene el hilo del filósofo

    int getId() const;             // - Retorna el ID del filósofo
    std::string getNombre() const; // - Retorna el nombre del filósofo
    int getVecesComio() const;     // - Retorna cuántas veces ha comido
};

#endif // - Fin del bloque condicional que evita doble inclusión