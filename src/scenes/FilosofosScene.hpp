#ifndef FILOSOFOS_SCENE_HPP // - Evitamos que este archivo se incluya más de una vez
#define FILOSOFOS_SCENE_HPP // - Definimos el macro que indica que ya fue incluido

#include "Scene.hpp"                            // - Incluimos la clase base Scene
#include "../filosofos/FilosofosController.hpp" // - Incluimos el controlador de filósofos
#include "raylib.h"                             // - Incluimos raylib para el tipo Font
#include <string>                               // - Incluimos string para textos

class Application; // - Forward declaration de Application

class FilosofosScene : public Scene
{
private:
    Application *app;               // - Puntero a la aplicación
    FilosofosController controller; // - Controlador que gestiona la mesa y los hilos
    Font fuente;                    // - Fuente personalizada JetBrains Mono
    float timer;                    // - Acumulador de tiempo para animaciones sinusoidales

    // - Wrapper de DrawTextEx para simplificar el uso de la fuente
    void texto(const char *txt, float x, float y, float size, Color color) const;

    // - Dibuja un filósofo en su posición circular alrededor de la mesa
    void drawFilosofo(int i, int n, float cx, float cy, float radio, const Filosofo &f) const;

    // - Dibuja un tenedor entre dos filósofos
    void drawTenedor(int i, int n, float cx, float cy, float radio, const Tenedor &t) const;

public:
    FilosofosScene(Application *app); // - Constructor
    ~FilosofosScene();                // - Destructor: libera la fuente

    void update() override; // - Lógica e input de la escena
    void draw() override;   // - Renderizado de la escena
};

#endif // - Fin del bloque condicional que evita doble inclusión