#ifndef PC_CONTROLLER_HPP
#define PC_CONTROLLER_HPP
#include "Buffer.hpp"     // - Incluimos Buffer, el recurso compartido central del sistema
#include "Productor.hpp"  // - Incluimos Productor, el actor que genera números
#include "Consumidor.hpp" // - Incluimos Consumidor, los actores que procesan números
#include <vector>         // - Incluimos vector para manejar múltiples consumidores
#include <memory>         // - Incluimos memory para usar unique_ptr y manejar memoria automáticamente
#include <string>         // - Incluimos string para la ruta del archivo

// --------------------------------------------------
// Funciones predicado globales
// --------------------------------------------------
bool esPrimo(int n); // - Retorna true si n es primo (tiene prioridad sobre par/impar)
bool esPar(int n);   // - Retorna true si n es par y no es primo
bool esImpar(int n); // - Retorna true si n es impar y no es primo

class PCController
{
private:
    Buffer buffer;                                         // - Buffer compartido entre el productor y los consumidores
    std::unique_ptr<Productor> productor;                  // - Puntero único al productor del sistema
    std::vector<std::unique_ptr<Consumidor>> consumidores; // - Lista de consumidores del sistema

    void iniciarConsumidores(); // - Crea e inicia los tres consumidores estándar

public:
    PCController(int bufferSize = 10);               // - Constructor vacío: sin archivo, espera carga manual
    void cargarArchivo(const std::string &filePath); // - Inicia o reinicia el productor con un archivo nuevo
    bool tieneArchivo() const;                       // - Retorna true si ya hay un productor activo

    Buffer *getBuffer();                                         // - Retorna puntero al buffer
    Productor *getProductor();                                   // - Retorna puntero al productor
    std::vector<std::unique_ptr<Consumidor>> &getConsumidores(); // - Retorna referencia a los consumidores
};
#endif