#include "Tenedor.hpp" // - Incluimos el header del Tenedor con sus declaraciones

// --------------------------------------------------
// Constructor
// --------------------------------------------------
Tenedor::Tenedor(int id)
    : id(id),       // - Guardamos el identificador del tenedor
      enUso(false), // - El tenedor inicia libre
      usadoPor(-1)  // - Sin filósofo usando el tenedor al inicio
{
}

// --------------------------------------------------
// Intenta tomar el tenedor sin bloquear
// --------------------------------------------------
bool Tenedor::intentarTomar()
{
    return mtx.try_lock(); // - Intentamos adquirir el mutex sin bloquear; retorna true si lo conseguimos
}

// --------------------------------------------------
// Libera el tenedor
// --------------------------------------------------
void Tenedor::soltar()
{
    enUso = false; // - Marcamos el tenedor como libre para la animación
    usadoPor = -1; // - Indicamos que ningún filósofo lo está usando
    mtx.unlock();  // - Liberamos el mutex para que otro filósofo pueda tomarlo
}

// --------------------------------------------------
// Getter del ID
// --------------------------------------------------
int Tenedor::getId() const
{
    return id; // - Retornamos el identificador del tenedor
}