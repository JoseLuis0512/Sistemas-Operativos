#ifndef MENU_SCENE_HPP // - Si no está definido, evita múltiples definiciones.
#define MENU_SCENE_HPP // - Se define en caso de cumplir la condición anterior.

#include "Scene.hpp" // - Incluimos la clase padre.
#include "raylib.h"  // - Incluimos raylib para el tipo Font.

class Application; // - Le indicamos que existe una clase "Application".

class MenuScene : public Scene
{
    // - AREA PRIVADA.
private:
    Application *app; // - Referencia a la aplicación para cambiar las escenas.
    Font fuente;      // - Fuente personalizada compartida con el resto de escenas.

    // - Wrapper interno para simplificar el uso de DrawTextEx en toda la escena.
    void texto(const char *txt, float x, float y, float size, Color color) const;

    // - AREA PÚBLICA.
public:
    MenuScene(Application *app); // - Constructor: carga la fuente.
    ~MenuScene();                // - Destructor: libera la fuente de memoria.

    // - Métodos obligatorios heredados de la clase padre.
    void update() override;
    void draw() override;
};
#endif // - Fin del bloque condicional.