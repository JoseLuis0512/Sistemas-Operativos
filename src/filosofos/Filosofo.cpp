#include "Filosfo.hpp" // - Incluimos el header del Filósofo con sus declaraciones
#include <thread>      // - Incluimos thread para pausas con sleep_for
#include <chrono>      // - Incluimos chrono para definir duraciones de tiempo

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Filosofo::Filosofo(int id, const std::string &nombre, Tenedor *izq, Tenedor *der)
    : id(id),                           // - Guardamos el identificador del filósofo
      nombre(nombre),                   // - Guardamos el nombre del filósofo
      tenedorIzq(izq),                  // - Guardamos el puntero al tenedor izquierdo
      tenedorDer(der),                  // - Guardamos el puntero al tenedor derecho
      running(true),                    // - El hilo inicia activo
      vecesComio(0),                    // - Inicia sin haber comido
      estado(EstadoFilosofo::PENSANDO), // - Estado inicial: pensando
      intentando(false),                // - No está intentando adquirir tenedores al inicio
      bloqueado(false),                 // - No está bloqueado al inicio
      intentosFallidos(0)               // - Sin intentos fallidos al inicio
{
    hilo = std::thread(&Filosofo::run, this); // - Creamos el hilo que ejecutará el ciclo de vida
}

// --------------------------------------------------
// Destructor: detiene el hilo del filósofo
// --------------------------------------------------
Filosofo::~Filosofo()
{
    running = false; // - Indicamos al hilo que debe detenerse
    hilo.join();     // - Esperamos a que el hilo termine completamente
}

// --------------------------------------------------
// Ciclo de vida del filósofo: pensar → esperar → comer → repetir
// --------------------------------------------------
void Filosofo::run()
{
    while (running) // - El filósofo repite su ciclo mientras esté activo
    {
        // --------------------------------------------------
        // Fase 1: PENSANDO
        // --------------------------------------------------
        estado = EstadoFilosofo::PENSANDO; // - Actualizamos el estado a pensando
        intentando = false;                // - No está intentando adquirir recursos
        bloqueado = false;                 // - No está bloqueado
        intentosFallidos = 0;              // - Reiniciamos el contador de fallos

        // - El tiempo de pensar varía según el ID para evitar que todos intenten comer al mismo tiempo
        int msPensar = 1000 + (id % 3) * 300; // - Entre 1000 y 1900 ms según el ID
        std::this_thread::sleep_for(std::chrono::milliseconds(msPensar));

        if (!running)
            break; // - Verificamos si debemos detenernos antes de continuar

        // --------------------------------------------------
        // Fase 2: ESPERANDO — intenta adquirir ambos tenedores
        // --------------------------------------------------
        estado = EstadoFilosofo::ESPERANDO; // - Actualizamos el estado a esperando
        intentando = true;                  // - Marcamos que está intentando adquirir tenedores

        // - Estrategia para evitar deadlock: los filósofos con ID par toman izquierda primero,
        //   los impares toman derecha primero (asimetría para romper la espera circular)
        Tenedor *primero = (id % 2 == 0) ? tenedorIzq : tenedorDer; // - Primer tenedor a intentar
        Tenedor *segundo = (id % 2 == 0) ? tenedorDer : tenedorIzq; // - Segundo tenedor a intentar

        bool tieneAmbos = false; // - Flag que indica si logró adquirir ambos tenedores

        while (running && !tieneAmbos) // - Sigue intentando hasta conseguir ambos o detenerse
        {
            if (primero->intentarTomar()) // - Intenta tomar el primer tenedor sin bloquear
            {
                primero->enUso = true;  // - Marcamos el tenedor como en uso para la animación
                primero->usadoPor = id; // - Registramos qué filósofo lo está usando

                if (segundo->intentarTomar()) // - Intenta tomar el segundo tenedor sin bloquear
                {
                    segundo->enUso = true;  // - Marcamos el segundo tenedor como en uso
                    segundo->usadoPor = id; // - Registramos qué filósofo lo está usando
                    tieneAmbos = true;      // - Conseguimos ambos tenedores, salimos del while
                    intentosFallidos = 0;   // - Reiniciamos el contador de fallos
                }
                else
                {
                    // - No pudimos tomar el segundo: soltamos el primero para evitar deadlock
                    primero->enUso = false; // - Liberamos el flag de uso del primer tenedor
                    primero->usadoPor = -1; // - Indicamos que ya no lo usa nadie
                    primero->soltar();      // - Liberamos el mutex del primer tenedor

                    intentosFallidos++;                  // - Incrementamos el contador de intentos fallidos
                    bloqueado = (intentosFallidos >= 3); // - Marcamos bloqueo si lleva 3+ fallos

                    // - Esperamos un tiempo aleatorio antes de reintentar (backoff)
                    int msEspera = 100 + (id * 50) % 200; // - Entre 100 y 300 ms según el ID
                    std::this_thread::sleep_for(std::chrono::milliseconds(msEspera));
                }
            }
            else
            {
                intentosFallidos++;                  // - El primer tenedor tampoco estaba disponible
                bloqueado = (intentosFallidos >= 3); // - Marcamos bloqueo si lleva 3+ fallos

                int msEspera = 100 + (id * 50) % 200; // - Backoff antes de reintentar
                std::this_thread::sleep_for(std::chrono::milliseconds(msEspera));
            }
        }

        if (!running) // - Si nos detuvieron mientras esperábamos, soltamos tenedores si los tenemos
        {
            if (tieneAmbos)
            {
                primero->enUso = false;
                primero->usadoPor = -1;
                primero->soltar();
                segundo->enUso = false;
                segundo->usadoPor = -1;
                segundo->soltar();
            }
            break;
        }

        // --------------------------------------------------
        // Fase 3: COMIENDO
        // --------------------------------------------------
        estado = EstadoFilosofo::COMIENDO; // - Actualizamos el estado a comiendo
        intentando = false;                // - Ya no está intentando, está comiendo
        bloqueado = false;                 // - Ya no está bloqueado
        vecesComio++;                      // - Incrementamos el contador de veces que comió

        std::this_thread::sleep_for(std::chrono::milliseconds(800)); // - Come durante 800 ms

        // - Soltamos ambos tenedores al terminar de comer
        primero->enUso = false;
        primero->usadoPor = -1;
        primero->soltar(); // - Liberamos el primer tenedor
        segundo->enUso = false;
        segundo->usadoPor = -1;
        segundo->soltar(); // - Liberamos el segundo tenedor
    }
}

// --------------------------------------------------
// Getters
// --------------------------------------------------
int Filosofo::getId() const
{
    return id; // - Retornamos el identificador del filósofo
}

std::string Filosofo::getNombre() const
{
    return nombre; // - Retornamos el nombre del filósofo
}

int Filosofo::getVecesComio() const
{
    return vecesComio; // - Retornamos cuántas veces ha comido el filósofo
}