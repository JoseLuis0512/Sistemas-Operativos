#include "MenuScene.hpp"          // - Incluimos el encabezado de MenuScene.
#include "../app/Application.hpp" // - Incluimos la clase Application para cambiar escenas.
#include "raylib.h"               // - Librería para la parte gráfica.

#include "PCScene.hpp" // - Incluimos PCScene para poder navegar a ella.
// #include "FilosofosScene.hpp"  // - Se habilitará cuando FilosofosScene esté lista.

// --------------------------------------------------
// Constructor: carga la fuente personalizada
// --------------------------------------------------
MenuScene::MenuScene(Application *app)
{
    this->app = app; // - Guardamos el puntero a la aplicación.

    fuente = LoadFont("assets/fonts/JetBrainsMono-VariableFont_wght.ttf"); // - Cargamos la fuente JetBrains Mono.
    SetTextureFilter(fuente.texture, TEXTURE_FILTER_BILINEAR);             // - Aplicamos filtro bilineal para suavizar la fuente.
}

// --------------------------------------------------
// Destructor: libera la fuente de la memoria de la GPU
// --------------------------------------------------
MenuScene::~MenuScene()
{
    UnloadFont(fuente); // - Liberamos la fuente para evitar memory leaks.
}

// --------------------------------------------------
// Wrapper interno: simplifica el uso de DrawTextEx
// --------------------------------------------------
void MenuScene::texto(const char *txt, float x, float y, float size, Color color) const
{
    DrawTextEx(fuente,       // - Fuente personalizada cargada en el constructor.
               txt,          // - Texto a dibujar.
               {x, y},       // - Posición en pantalla como Vector2.
               size,         // - Tamaño de la fuente en píxeles.
               size * 0.05f, // - Espaciado entre caracteres proporcional al tamaño.
               color);       // - Color del texto.
}

// --------------------------------------------------
// Update: manejo del input del usuario
// --------------------------------------------------
void MenuScene::update()
{
    Vector2 mouse = GetMousePosition(); // - Obtenemos la posición actual del cursor.

    Rectangle btnPC = {190, 200, 200, 50};        // - Rectángulo del botón PC.
    Rectangle btnFilosofos = {410, 200, 200, 50}; // - Rectángulo del botón Filósofos.

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) // - Detectamos click izquierdo.
    {
        if (CheckCollisionPointRec(mouse, btnPC)) // - Click en Productor/Consumidor.
        {
            app->setScene(new PCScene(app)); // - Navegamos a la escena del Productor/Consumidor.
            return;                          // - Salimos inmediatamente para no procesar más botones.
        }
        else if (CheckCollisionPointRec(mouse, btnFilosofos)) // - Click en Filósofos/Comensales.
        {
            // app->setScene(new FilosofosScene(app)); // - Se habilitará cuando esté lista.
        }
    }
}

// --------------------------------------------------
// Draw: renderizado del menú principal
// --------------------------------------------------
void MenuScene::draw()
{
    // --------------------------------------------------
    // Título
    // --------------------------------------------------
    texto("PROYECTO SISTEMAS OPERATIVOS", 180, 100, 32, BLACK); // - Título principal del menú.

    // --------------------------------------------------
    // Botones
    // --------------------------------------------------
    Rectangle btnPC = {190, 200, 200, 50};        // - Rectángulo del botón PC.
    Rectangle btnFilosofos = {410, 200, 200, 50}; // - Rectángulo del botón Filósofos.

    Vector2 mouse = GetMousePosition(); // - Posición del cursor para efectos hover.

    // --------------------------------------------------
    // Botón Productor/Consumidor
    // --------------------------------------------------
    Color colorPC = CheckCollisionPointRec(mouse, btnPC) ? DARKGRAY : LIGHTGRAY; // - Color hover.
    DrawRectangleRec(btnPC, colorPC);                                            // - Fondo del botón.
    DrawRectangleLinesEx(btnPC, 2, DARKGRAY);                                    // - Borde del botón.
    texto("Productor/Consumidor", 205, 210, 18, BLACK);                          // - Texto del botón.

    // --------------------------------------------------
    // Botón Filósofos/Comensales
    // --------------------------------------------------
    Color colorFil = CheckCollisionPointRec(mouse, btnFilosofos) ? DARKGRAY : LIGHTGRAY; // - Color hover.
    DrawRectangleRec(btnFilosofos, colorFil);                                            // - Fondo del botón.
    DrawRectangleLinesEx(btnFilosofos, 2, DARKGRAY);                                     // - Borde del botón.
    texto("Filosofos/Comensales", 425, 210, 18, BLACK);                                  // - Texto del botón.

    // --------------------------------------------------
    // Borde rojo al hacer hover sobre cualquier botón
    // --------------------------------------------------
    if (CheckCollisionPointRec(mouse, btnPC))
        DrawRectangleLinesEx(btnPC, 3, RED); // - Borde rojo hover PC.

    if (CheckCollisionPointRec(mouse, btnFilosofos))
        DrawRectangleLinesEx(btnFilosofos, 3, RED); // - Borde rojo hover Filósofos.

    // --------------------------------------------------
    // Miembros del equipo de trabajo.
    // --------------------------------------------------
    texto("David Alconero Lepe         - 1216123", 20, 450, 15, BLACK);
    texto("Jose Luis Enriquez Barillas - 1053223", 20, 475, 15, BLACK);
    texto("Anthony Hernandez Perez     - 1086223", 20, 500, 15, BLACK);
    texto("Ana Sofia Siguenza Aguilar  - 1333123", 20, 525, 15, BLACK);
}