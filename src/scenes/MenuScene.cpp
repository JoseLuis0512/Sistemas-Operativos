#include "MenuScene.hpp"          // - Incluimos el encabezado.
#include "../app/Application.hpp" // - Incluimos la clase "Application"
#include "raylib.h"               // - Librería para la parte gráfica.

// Estas clases las crearemos después
#include "PCScene.hpp"
// #include "FilosofosScene.hpp"

// - Constructor
MenuScene::MenuScene(Application *app)
{
    this->app = app;
}

// - Update (Manejo del input)
void MenuScene::update()
{
    Vector2 mouse = GetMousePosition(); // - Obtenemos la posición del cursor.

    // - Definimos los botones como rectángulos.
    Rectangle btnPC = {300, 200, 200, 50};
    Rectangle btnFilosofos = {300, 200, 200, 50};

    // -  Lógica para detectar el click del ratón.
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // - Click en Productos-Consumidor.
        if (CheckCollisionPointRec(mouse, btnPC))
        {

            app->setScene(new PCScene(app));
        }

        // - Click en Filosofos-Comensales
        if (CheckCollisionPointRec(mouse, btnFilosofos))
        {
            // app-setScene(new FilosofosScene(app))
        }
    }
}

// - Renderizado
void MenuScene::draw()
{
    // - Título
    DrawText("PROYECTO SISTEMAS OPERATIVOS", 120, 100, 30, BLACK);

    // - Botones
    Rectangle btnPC = {300, 200, 200, 50};
    Rectangle btnFilosofos = {300, 400, 200, 50};

    // - Dibujamos los botones.
    DrawRectangleRec(btnPC, LIGHTGRAY);
    DrawRectangleRec(btnFilosofos, LIGHTGRAY);

    // - Texto de botones.
    DrawText("Productor/Consumidor", 310, 215, 15, BLACK);
    DrawText("Filósofos/Comensales", 310, 415, 15, BLACK);

    // - Hover de los botones.
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, btnPC))
    {
        DrawRectangleLinesEx(btnPC, 3, RED);
    }

    if (CheckCollisionPointRec(mouse, btnFilosofos))
    {
        DrawRectangleLinesEx(btnFilosofos, 3, RED);
    }
}