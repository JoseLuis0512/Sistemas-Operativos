#include "Application.hpp"         // - Incluimos el header.
#include "raylib.h"                // - Incluimos la librería RayLib para la interfaz y animaciones.
#include "../scenes/MenuScene.hpp" // - Incluimos la escena del menu.

// - Constructor.
Application::Application()
{
    InitWindow(1024, 768, "Proyecto Sistemas Operativos"); // - Inicializamos RayLib.
    SetTargetFPS(60);                                      // - Configuramos a 60 fps.
    currentScene = new MenuScene(this);                    // - Inicializamos la escena principal.
    pendingScene = nullptr;                                // - Sin escena pendiente al inicio.
}

// - Destructor.
Application::~Application()
{
    delete currentScene; // - Liberamos memoria de la escena actual.
    delete pendingScene; // - Liberamos la escena pendiente si quedó alguna.
    CloseWindow();       // - Cerramos la ventana.
}

// - Cambio de escena: guarda la nueva escena para aplicarla al final del frame.
void Application::setScene(Scene *newScene)
{
    if (pendingScene != nullptr) // - Si ya hay una escena pendiente, ignoramos la nueva
    {
        delete newScene; // - Liberamos la escena nueva para evitar memory leak
        return;
    }
    pendingScene = newScene; // - Guardamos la escena nueva sin destruir la actual todavía
}

// - Loop Principal.
void Application::run()
{
    int frameCooldown = 0; // - Frames de espera tras un cambio de escena para ignorar clicks residuales

    while (!WindowShouldClose())
    {
        if (frameCooldown > 0) // - Durante el cooldown no procesamos update para evitar clicks residuales
        {
            frameCooldown--;
        }
        else
        {
            currentScene->update(); // - Actualizamos la escena actual solo fuera del cooldown
        }

        // - Si hay una escena pendiente la aplicamos aquí entre update y draw.
        if (pendingScene != nullptr)
        {
            delete currentScene;         // - Ahora es seguro destruir la escena anterior.
            currentScene = pendingScene; // - Activamos la nueva escena.
            pendingScene = nullptr;      // - Limpiamos el puntero pendiente.
            frameCooldown = 5;           // - Esperamos 5 frames antes de procesar input en la nueva escena.
        }

        // - Proceso de dibujo.
        BeginDrawing();            // - Empezamos el dibujo.
        ClearBackground(RAYWHITE); // - Borramos el fondo.
        currentScene->draw();      // - Función de dibujo.
        EndDrawing();              // - Finalizamos el dibujo.
    }
}