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
      vecesComio(0),                    // - Inicia sin haber comido
      running(true),                    // - El hilo inicia activo
      estado(EstadoFilosofo::PENSANDO), // - Estado inicial: pensando
      intentando(false),                // - No está intentando adquirir tenedores al inicio
      bloqueado(false),                 // - No está bloqueado al inicio
      intentosFallidos(0),              // - Sin intentos fallidos consecutivos al inicio
      totalIntentosFallidos(0)          // - Sin intentos fallidos acumulados al inicio
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
// Ciclo de vida: pensar → esperar → comer → repetir
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
        intentosFallidos = 0;              // - Reiniciamos intentos fallidos consecutivos

        int msPensar = 1000 + (id % 3) * 300; // - Tiempo de pensar varía por ID (1000-1900 ms)
        std::this_thread::sleep_for(std::chrono::milliseconds(msPensar));

        if (!running)
            break; // - Verificamos si debemos detenernos

        // --------------------------------------------------
        // Fase 2: ESPERANDO — intenta adquirir ambos tenedores
        // --------------------------------------------------
        estado = EstadoFilosofo::ESPERANDO; // - Actualizamos el estado a esperando
        intentando = true;                  // - Marcamos que está intentando

        // - Asimetría para evitar deadlock: pares toman izquierda primero, impares toman derecha primero
        Tenedor *primero = (id % 2 == 0) ? tenedorIzq : tenedorDer;
        Tenedor *segundo = (id % 2 == 0) ? tenedorDer : tenedorIzq;

        bool tieneAmbos = false; // - Flag de éxito en la adquisición

        while (running && !tieneAmbos)
        {
            if (primero->intentarTomar()) // - Intenta tomar el primer tenedor sin bloquear
            {
                primero->enUso = true;
                primero->usadoPor = id;

                if (segundo->intentarTomar()) // - Intenta tomar el segundo tenedor
                {
                    segundo->enUso = true;
                    segundo->usadoPor = id;
                    tieneAmbos = true;    // - Éxito: tiene ambos tenedores
                    intentosFallidos = 0; // - Reiniciamos consecutivos
                }
                else
                {
                    // - Suelta el primero para evitar deadlock
                    primero->enUso = false;
                    primero->usadoPor = -1;
                    primero->soltar();

                    intentosFallidos++;                  // - Incrementamos consecutivos
                    totalIntentosFallidos++;             // - Incrementamos acumulado total
                    bloqueado = (intentosFallidos >= 3); // - Marcamos bloqueo si lleva 3+ fallos

                    int msEspera = 100 + (id * 50) % 200;
                    std::this_thread::sleep_for(std::chrono::milliseconds(msEspera));
                }
            }
            else
            {
                intentosFallidos++;      // - Primer tenedor no disponible
                totalIntentosFallidos++; // - Incrementamos acumulado total
                bloqueado = (intentosFallidos >= 3);

                int msEspera = 100 + (id * 50) % 200;
                std::this_thread::sleep_for(std::chrono::milliseconds(msEspera));
            }
        }

        if (!running) // - Si nos detuvieron mientras esperábamos, soltamos si tenemos tenedores
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
        intentando = false;                // - Ya no está intentando
        bloqueado = false;                 // - Ya no está bloqueado
        vecesComio++;                      // - Incrementamos el contador de comidas

        std::this_thread::sleep_for(std::chrono::milliseconds(800)); // - Come durante 800 ms

        // - Soltamos ambos tenedores al terminar
        primero->enUso = false;
        primero->usadoPor = -1;
        primero->soltar();
        segundo->enUso = false;
        segundo->usadoPor = -1;
        segundo->soltar();
    }

    estado = EstadoFilosofo::PENSANDO; // - Volvemos a pensando al terminar el hilo
}

// --------------------------------------------------
// Getters
// --------------------------------------------------
int Filosofo::getId() const
{
    return id; // - Retornamos el identificador
}

std::string Filosofo::getNombre() const
{
    return nombre; // - Retornamos el nombre
}

int Filosofo::getVecesComio() const
{
    return vecesComio; // - Retornamos cuántas veces comió
}