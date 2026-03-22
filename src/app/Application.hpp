#ifndef APLICACION_HPP // - Verificar si SCENE_HPP no está definido previamente, evita definiciones múltiples.
#define APLICATION_HPP // - Si no está definio, entonces se define.

#include "../scenes/Scene.hpp" // - Incluimos la clase Padre.

// La clase maneja la ventana, el loop principal y la escena actual.
class Application
{
    // - AREA PÚBLICA.
private:
    Scene *currentScene; // - Puntero a la escena actual.
    Scene *pendingScene; // - Escena pendiente a aplicar al final del frame actual.
    // - AREA PRIVADA.
public:
    Application();                  // - Constructor.
    ~Application();                 // - Destructor.
    void run();                     // Ejecuta la aplicación.
    void setScene(Scene *newScene); // Permite cambiar de escena.
};
#endif