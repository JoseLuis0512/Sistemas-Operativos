#include "PCScene.hpp"                // - Incluimos el header de PCScene con sus declaraciones
#include "../app/Application.hpp"     // - Incluimos Application para poder interactuar con la app
#include "raylib.h"                   // - Incluimos raylib para funciones gráficas y de tiempo
#include "../utils/tinyfiledialogs.h" // - Incluimos tinyfiledialogs para el diálogo de archivo nativo sin conflictos con raylib

// --------------------------------------------------
// Constructor
// --------------------------------------------------
PCScene::PCScene(Application *app)
    : controller(10) // - Inicializamos el controlador solo con el tamaño del buffer, sin archivo aún
{
    this->app = app;    // - Guardamos el puntero a la aplicación en el atributo de instancia
    this->timer = 0.0f; // - Inicializamos el acumulador de tiempo en cero
}

// --------------------------------------------------
// Abre el diálogo nativo del sistema para seleccionar un .txt
// --------------------------------------------------
std::string PCScene::abrirDialogoArchivo()
{
    // - Definimos los filtros de archivo aceptados (solo .txt)
    const char *filtros[] = {"*.txt"};

    // - Llamamos a tinyfiledialogs para abrir el diálogo nativo del sistema operativo
    const char *resultado = tinyfd_openFileDialog(
        "Seleccionar archivo de numeros", // - Título del diálogo
        "",                               // - Ruta inicial (vacía = carpeta actual)
        1,                                // - Número de filtros
        filtros,                          // - Array de filtros
        "Archivos de texto (*.txt)",      // - Descripción del filtro
        0                                 // - 0 = selección de un solo archivo
    );

    if (resultado)                     // - Si el usuario seleccionó un archivo (no canceló)
        return std::string(resultado); // - Retornamos la ruta como string

    return ""; // - Si el usuario canceló, retornamos cadena vacía
}

// --------------------------------------------------
// Update: detecta click en botón y acumula tiempo
// --------------------------------------------------
void PCScene::update()
{
    timer += GetFrameTime(); // - Acumulamos el tiempo transcurrido desde el último frame

    Rectangle btnCargar = {300, 500, 200, 40}; // - Rectángulo del botón de carga
    Vector2 mouse = GetMousePosition();        // - Posición actual del cursor

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && // - Detectamos click izquierdo
        CheckCollisionPointRec(mouse, btnCargar))  // - Verificamos que fue sobre el botón
    {
        std::string ruta = abrirDialogoArchivo(); // - Abrimos el diálogo para seleccionar el archivo

        if (!ruta.empty()) // - Si el usuario seleccionó un archivo válido
        {
            archivoActual = ruta;           // - Guardamos la ruta para mostrarla en pantalla
            controller.cargarArchivo(ruta); // - Iniciamos el productor con el archivo seleccionado
        }
    }
}

// --------------------------------------------------
// Draw: dibuja el buffer, estados, sumas y botón
// --------------------------------------------------
void PCScene::draw()
{
    // --------------------------------------------------
    // Título
    // --------------------------------------------------
    DrawText("PRODUCTOR - CONSUMIDOR", 250, 50, 30, BLACK); // - Dibujamos el título de la escena

    // --------------------------------------------------
    // Botón para cargar archivo
    // --------------------------------------------------
    Rectangle btnCargar = {300, 500, 200, 40}; // - Rectángulo del botón de carga
    Vector2 mouse = GetMousePosition();        // - Posición del cursor para efecto hover
    Color colorBtn = CheckCollisionPointRec(mouse, btnCargar)
                         ? DARKGRAY
                         : LIGHTGRAY; // - Color más oscuro al pasar el cursor

    DrawRectangleRec(btnCargar, colorBtn);           // - Dibujamos el fondo del botón
    DrawRectangleLinesEx(btnCargar, 2, BLACK);       // - Dibujamos el borde del botón
    DrawText("Cargar archivo", 325, 512, 18, BLACK); // - Texto del botón

    // --------------------------------------------------
    // Nombre del archivo cargado
    // --------------------------------------------------
    if (archivoActual.empty()) // - Si aún no se cargó ningún archivo
    {
        DrawText("Sin archivo cargado", 270, 555, 16, GRAY); // - Indicamos que no hay archivo
    }
    else
    {
        std::string nombre = archivoActual.substr(archivoActual.find_last_of("\\/") + 1); // - Extraemos solo el nombre del archivo
        DrawText(TextFormat("Archivo: %s", nombre.c_str()), 200, 555, 16, DARKGREEN);     // - Mostramos el nombre
    }

    // --------------------------------------------------
    // Si no hay archivo, mostramos instrucción y salimos
    // --------------------------------------------------
    if (!controller.tieneArchivo())
    {
        DrawText("Carga un archivo .txt con numeros separados por comas",
                 100, 280, 18, GRAY); // - Instrucción al usuario
        return;                       // - No hay nada más que dibujar
    }

    // --------------------------------------------------
    // Obtener snapshot del buffer para dibujarlo
    // --------------------------------------------------
    Buffer *buf = controller.getBuffer();        // - Puntero al buffer compartido
    std::vector<int> snapshot = buf->snapshot(); // - Copia segura del buffer para dibujar
    int maxSize = buf->getMaxSize();             // - Tamaño máximo del buffer
    Productor *prod = controller.getProductor(); // - Puntero al productor

    // --------------------------------------------------
    // Dibujar celdas del buffer
    // --------------------------------------------------
    int startX = 100; // - Posición X inicial de las celdas
    int startY = 150; // - Posición Y inicial de las celdas
    int boxSize = 50; // - Tamaño en píxeles de cada celda

    for (int i = 0; i < maxSize; i++) // - Iteramos por cada posición posible del buffer
    {
        Rectangle box = {
            (float)(startX + i * (boxSize + 10)), // - Posición X con espaciado entre celdas
            (float)startY,                        // - Posición Y fija
            (float)boxSize,                       // - Ancho de la celda
            (float)boxSize                        // - Alto de la celda
        };

        if (i < (int)snapshot.size()) // - Si hay un elemento en esta posición lo dibujamos
        {
            int val = snapshot[i]; // - Valor del elemento
            Color color = GREEN;   // - Color por defecto (fallback)

            if (esPrimo(val)) // - Primo: rojo (prioridad más alta)
                color = RED;
            else if (esPar(val)) // - Par no primo: azul
                color = BLUE;
            else // - Impar no primo: naranja
                color = ORANGE;

            DrawRectangleRec(box, color);                                       // - Celda con color
            DrawText(TextFormat("%d", val), box.x + 10, box.y + 15, 20, BLACK); // - Número dentro
        }
        else
        {
            DrawRectangleLinesEx(box, 2, GRAY); // - Celda vacía: solo contorno
        }
    }

    // --------------------------------------------------
    // Leyenda de colores
    // --------------------------------------------------
    DrawRectangle(100, 220, 20, 20, RED);
    DrawText("Primo", 125, 222, 16, BLACK); // - Leyenda primo
    DrawRectangle(200, 220, 20, 20, BLUE);
    DrawText("Par", 225, 222, 16, BLACK); // - Leyenda par
    DrawRectangle(280, 220, 20, 20, ORANGE);
    DrawText("Impar", 305, 222, 16, BLACK); // - Leyenda impar

    // --------------------------------------------------
    // Estado del productor
    // --------------------------------------------------
    Color colorProd = prod->isBlocked() ? RED : GREEN;                       // - Rojo si bloqueado
    DrawText("Productor:", 100, 260, 20, BLACK);                             // - Etiqueta
    DrawText(prod->isBlocked() ? "BLOQUEADO (buffer lleno)" : "PRODUCIENDO", // - Estado textual
             220, 260, 20, colorProd);

    // --------------------------------------------------
    // Ocupación del buffer
    // --------------------------------------------------
    DrawText(TextFormat("Buffer: %d / %d", (int)snapshot.size(), maxSize),
             100, 290, 20, BLACK); // - Mostramos ocupación actual

    // --------------------------------------------------
    // Estado y sumas de cada consumidor
    // --------------------------------------------------
    auto &consumidores = controller.getConsumidores(); // - Lista de consumidores
    int offsetY = 330;                                 // - Posición Y inicial

    for (auto &c : consumidores) // - Iteramos sobre cada consumidor
    {
        Color colorC = c->isBlocked() ? GRAY : GREEN;        // - Gris si bloqueado
        DrawText(TextFormat("%-8s", c->getNombre().c_str()), // - Nombre del consumidor
                 100, offsetY, 20, BLACK);
        DrawText(c->isBlocked() ? "ESPERANDO" : "ACTIVO", // - Estado
                 230, offsetY, 20, colorC);
        DrawText(TextFormat("Suma: %d", c->getSuma()), // - Suma acumulada
                 370, offsetY, 20, BLACK);
        offsetY += 30; // - Siguiente consumidor
    }
}