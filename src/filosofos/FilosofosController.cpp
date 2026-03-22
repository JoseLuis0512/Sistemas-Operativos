#include "FilosofosController.hpp" // - Incluimos el header del controlador con sus declaraciones

// --------------------------------------------------
// Constructor
// --------------------------------------------------
FilosofosController::FilosofosController(int n)
    : mesa(n) // - Inicializamos la mesa con n filósofos y n tenedores
{
}

// --------------------------------------------------
// Getter de la mesa
// --------------------------------------------------
Mesa &FilosofosController::getMesa()
{
    return mesa; // - Retornamos referencia a la mesa para que la escena la consulte
}