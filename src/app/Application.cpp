#include "Application.hpp"         // - Incluimos el header.
#include "raylib.h"                // - Incluimos la librería RayLib para la interfaz y animaciones.
#include "../scenes/MenuScene.hpp" // - Incluimos la escena del menu.

// - Constructor.
Application::Application()
{
    InitWindow(800, 600, "Proyecto Sistemas Operativos"); // - Inicializamos RayLib.
    SetTargetFPS(60);                                     // - Configuramos a 60 fps.
    currentScene = new MenuScene(this);                   // - Inicializamos la escena principal.
}

// - Destructor
Application::~Application()
{
    delete currenScene; // - Liberamos memoria de la escena actual.
    CloseWindow();      // - Cerramos la ventana.
}

// - Cambio de escena.
void Application::setScene(Scene *newScene)
{
    delete currenScene;      // - Eliminamos la escena anterior.
    currentScene = newScene; // - Asignamos la nueva escena.
}

// - Loop Principal.
void Application::run()
{
    while (!WindowsShouldClose())
    {
        currentScene->update(); // - Actualizamos la escena.

        // - Proceso de dibujo
        BeginDrawing();            // - Empezamos el dibujo.
        ClearBackground(RAYWHITE); // - Borramos el fondo.
        currentScene->draw();      // - Función de dibujo.
        EndDrawing();              // - Finalizamos el dibujo.
    }
}