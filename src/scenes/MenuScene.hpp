#ifndef MENU_SCENE_HPP // - Si no está definido, evita múltiples definiciones.
#define MENU_SCENE_HPP // - Se define en caso de cumplir la condición anterior.

#include "Scene.hpp" // - Incluimos la clase padre.

class Application; // - Le indicamos que existe una clase "Application".

class MenuScene : public Scene
{
    // - AREA PRIVADA.
private:
    Application *app; // - Referencia a la aplicación para cambiar las escenas.

    // - AREA PÚBLICA.
public:
    MenuScene(Application *app); // - Constructor.

    // Métodos obligatorios (Herados de la clase padre).
    void update() override;
    void draw() override;
};
#endif // - Fin del bloque condicional.