#ifndef FILOSOFOS_CONTROLLER_HPP // - Evitamos que este archivo se incluya más de una vez
#define FILOSOFOS_CONTROLLER_HPP // - Definimos el macro que indica que ya fue incluido

#include "Mesa.hpp" // - Incluimos Mesa que gestiona filósofos y tenedores

class FilosofosController
{
private:
    Mesa mesa; // - Mesa con los filósofos y tenedores del sistema

public:
    FilosofosController(int n = 5); // - Constructor: inicializa la mesa con n filósofos (5 por defecto)

    Mesa &getMesa(); // - Retorna referencia a la mesa para que la escena acceda a filósofos y tenedores
};

#endif // - Fin del bloque condicional que evita doble inclusión