#ifndef FILOSOFOS_SCENE_HPP // - Evitamos que este archivo se incluya más de una vez
#define FILOSOFOS_SCENE_HPP // - Definimos el macro que indica que ya fue incluido

#include "Scene.hpp"                            // - Incluimos la clase base Scene
#include "../filosofos/FilosofosController.hpp" // - Incluimos el controlador de filósofos
#include "raylib.h"                             // - Incluimos raylib para el tipo Font
#include <memory>                               // - Incluimos memory para unique_ptr del controlador
#include <string>                               // - Incluimos string para textos

class Application; // - Forward declaration de Application

// - Estados de la escena
enum class EstadoEscena
{
    CONFIGURANDO, // - El usuario está eligiendo parámetros
    EJECUTANDO,   // - La simulación está corriendo
    RESULTADOS    // - La simulación terminó y se muestran los resultados
};

class FilosofosScene : public Scene
{
private:
    Application *app; // - Puntero a la aplicación
    Font fuente;      // - Fuente personalizada JetBrains Mono
    float timer;      // - Acumulador de tiempo para animaciones

    EstadoEscena estadoEscena;                       // - Estado actual de la escena
    int nFilosofos;                                  // - Número de filósofos seleccionado
    int duracionSeg;                                 // - Duración de la simulación en segundos
    float tiempoRestante;                            // - Tiempo restante de simulación
    float tiempoTotal;                               // - Tiempo total transcurrido al terminar
    std::unique_ptr<FilosofosController> controller; // - Controlador creado al iniciar

    // - Wrapper de DrawTextEx para simplificar el uso de la fuente
    void texto(const char *txt, float x, float y, float size, Color color) const;

    // - Pantallas según estado
    void drawConfiguracion(); // - Pantalla de configuración de parámetros
    void drawEjecucion();     // - Pantalla de simulación en curso
    void drawResultados();    // - Pantalla de resultados finales

    // - Helpers de dibujo de la simulación
    void drawFilosofo(int i, int n, float cx, float cy, float radio, const Filosofo &f) const;
    void drawTenedor(int i, int n, float cx, float cy, float radio, const Tenedor &t) const;

public:
    FilosofosScene(Application *app); // - Constructor: inicia en configuración
    ~FilosofosScene();                // - Destructor: libera la fuente

    void update() override; // - Lógica e input según el estado actual
    void draw() override;   // - Renderizado según el estado actual
};

#endif // - Fin del bloque condicional que evita doble inclusión