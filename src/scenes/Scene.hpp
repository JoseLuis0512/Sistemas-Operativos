#ifndef SCENE_HPP // - Verificar si SCENE_HPP no está definido previamente, evita definiciones múltiples.
#define SCENE_HPP // - Si no está definio, entonces se define.

// - Clase abstracta, representa una pantalla del sistema.
class Scene
{
    // - AREA PÚBLICA
public:
    /* Los métodos Virtuales se usaron para lograr polimorfismo, consiguiendo que los mismos metodos se comporten
    de manera diferente según la clase. */
    virtual ~Scene() {}        // - Destrcutor virtual para evitar problemas de memoria al usar punteros.
    virtual void update() = 0; // - Método de actualización, se maneja el input del usuario y la lógica.
    virtual void draw() = 0;   // - Renderizado de la pantalla.
};
#endif