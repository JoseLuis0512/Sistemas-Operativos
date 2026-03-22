#ifndef TENEDOR_HPP // - Evitamos que este archivo se incluya más de una vez
#define TENEDOR_HPP // - Definimos el macro que indica que ya fue incluido

#include <mutex>  // - Incluimos mutex para proteger el acceso exclusivo al tenedor
#include <atomic> // - Incluimos atomic para el flag de estado visible desde el hilo de render

class Tenedor
{
private:
    std::mutex mtx; // - Mutex que garantiza exclusión mutua sobre el tenedor
    int id;         // - Identificador único del tenedor

public:
    std::atomic<bool> enUso;   // - true si el tenedor está siendo usado por algún filósofo
    std::atomic<int> usadoPor; // - ID del filósofo que lo está usando (-1 si está libre)

    Tenedor(int id);      // - Constructor: inicializa el tenedor con su ID
    bool intentarTomar(); // - Intenta adquirir el tenedor sin bloquear; retorna true si lo consiguió
    void soltar();        // - Libera el tenedor para que otro filósofo pueda usarlo
    int getId() const;    // - Retorna el ID del tenedor
};

#endif // - Fin del bloque condicional que evita doble inclusión