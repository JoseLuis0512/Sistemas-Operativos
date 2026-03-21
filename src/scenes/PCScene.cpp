#include "PCScene.hpp"                // - Incluimos el header de PCScene con sus declaraciones
#include "../app/Application.hpp"     // - Incluimos Application para poder interactuar con la app
#include "raylib.h"                   // - Incluimos raylib para funciones gráficas y de tiempo
#include "../utils/tinyfiledialogs.h" // - Incluimos tinyfiledialogs para el diálogo de archivo nativo

// --------------------------------------------------
// Constructor
// --------------------------------------------------
PCScene::PCScene(Application *app)
    : controller(10) // - Inicializamos el controlador solo con el tamaño del buffer, sin archivo aún
{
    this->app = app;    // - Guardamos el puntero a la aplicación en el atributo de instancia
    this->timer = 0.0f; // - Inicializamos el acumulador de tiempo en cero

    fuente = LoadFont("assets/fonts/JetBrainsMono-VariableFont_wght.ttf"); // - Cargamos la fuente JetBrains Mono desde assets
    SetTextureFilter(fuente.texture, TEXTURE_FILTER_BILINEAR);             // - Aplicamos filtro bilineal para suavizar la fuente
}

// --------------------------------------------------
// Destructor: libera la fuente de la memoria de la GPU
// --------------------------------------------------
PCScene::~PCScene()
{
    UnloadFont(fuente); // - Liberamos la fuente cargada para evitar memory leaks
}

// --------------------------------------------------
// Wrapper interno: simplifica el uso de DrawTextEx en toda la escena
// --------------------------------------------------
void PCScene::texto(const char *txt, float x, float y, float size, Color color) const
{
    DrawTextEx(fuente,       // - Fuente personalizada cargada en el constructor
               txt,          // - Texto a dibujar
               {x, y},       // - Posición en pantalla como Vector2
               size,         // - Tamaño de la fuente en píxeles
               size * 0.05f, // - Espaciado entre caracteres proporcional al tamaño
               color);       // - Color del texto
}

// --------------------------------------------------
// Abre el diálogo nativo del sistema para seleccionar un .txt
// --------------------------------------------------
std::string PCScene::abrirDialogoArchivo()
{
    const char *filtros[] = {"*.txt"}; // - Filtro: solo archivos .txt

    const char *resultado = tinyfd_openFileDialog( // - Abrimos el diálogo nativo del sistema
        "Seleccionar archivo de numeros",          // - Título del diálogo
        "",                                        // - Ruta inicial vacía (carpeta actual)
        1,                                         // - Número de filtros
        filtros,                                   // - Array de filtros
        "Archivos de texto (*.txt)",               // - Descripción del filtro
        0                                          // - 0 = selección de un solo archivo
    );

    if (resultado)                     // - Si el usuario seleccionó un archivo
        return std::string(resultado); // - Retornamos la ruta completa como string

    return ""; // - Si el usuario canceló, retornamos cadena vacía
}

// --------------------------------------------------
// Update: detecta click en botón y acumula tiempo
// --------------------------------------------------
void PCScene::update()
{
    timer += GetFrameTime(); // - Acumulamos el tiempo transcurrido desde el último frame

    Rectangle btnCargar = {300, 450, 200, 40}; // - Rectángulo del botón
    Vector2 mouse = GetMousePosition();        // - Posición actual del cursor

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && // - Detectamos click izquierdo
        CheckCollisionPointRec(mouse, btnCargar))  // - Verificamos que fue sobre el botón
    {
        std::string ruta = abrirDialogoArchivo(); // - Abrimos el diálogo de selección

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
    texto("PRODUCTOR - CONSUMIDOR", 240, 40, 28, BLACK); // - Título principal de la escena

    // --------------------------------------------------
    // Botón para cargar archivo
    // --------------------------------------------------
    Rectangle btnCargar = {300, 450, 200, 40}; // - Rectángulo del botón
    Vector2 mouse = GetMousePosition();        // - Posición del cursor para hover
    Color colorBtn = CheckCollisionPointRec(mouse, btnCargar)
                         ? DARKGRAY
                         : LIGHTGRAY; // - Oscurece el botón al pasar el cursor

    DrawRectangleRec(btnCargar, colorBtn);        // - Fondo del botón
    DrawRectangleLinesEx(btnCargar, 2, DARKGRAY); // - Borde del botón
    texto("Cargar archivo", 335, 460, 18, BLACK); // - Texto del botón

    // --------------------------------------------------
    // Nombre del archivo cargado
    // --------------------------------------------------
    if (archivoActual.empty()) // - Sin archivo cargado aún
    {
        texto("Sin archivo cargado", 335, 495, 14, BLACK); // - Indicamos que no hay archivo
    }
    else
    {
        std::string nombre = archivoActual.substr(archivoActual.find_last_of("\\/") + 1); // - Solo el nombre del archivo
        texto(TextFormat("Archivo: %s", nombre.c_str()), 335, 495, 14, DARKGREEN);        // - Mostramos el nombre
    }

    // --------------------------------------------------
    // Si no hay archivo cargado mostramos instrucción y salimos
    // --------------------------------------------------
    if (!controller.tieneArchivo())
    {
        texto("Carga un archivo .txt con numeros separados por comas", 120, 280, 20, BLACK); // - Instrucción al usuario
        return;                                                                              // - No hay nada más que dibujar
    }

    // --------------------------------------------------
    // Obtener snapshot del buffer para dibujarlo
    // --------------------------------------------------
    Buffer *buf = controller.getBuffer();        // - Puntero al buffer compartido
    std::vector<int> snapshot = buf->snapshot(); // - Copia segura para dibujar sin bloquear
    int maxSize = buf->getMaxSize();             // - Tamaño máximo del buffer
    Productor *prod = controller.getProductor(); // - Puntero al productor

    // --------------------------------------------------
    // Dibujar celdas del buffer
    // --------------------------------------------------
    int startX = 100; // - Posición X inicial de las celdas
    int startY = 155; // - Posición Y inicial de las celdas
    int boxSize = 50; // - Tamaño en píxeles de cada celda

    for (int i = 0; i < maxSize; i++) // - Iteramos por cada posición posible del buffer
    {
        Rectangle box = {
            (float)(startX + i * (boxSize + 10)), // - Posición X con espaciado entre celdas
            (float)startY,                        // - Posición Y fija
            (float)boxSize,                       // - Ancho de la celda
            (float)boxSize                        // - Alto de la celda
        };

        if (i < (int)snapshot.size()) // - Si hay un elemento dibujamos con color
        {
            int val = snapshot[i]; // - Valor del elemento
            Color color = GREEN;   // - Color por defecto (fallback)

            if (esPrimo(val)) // - Primo: rojo (prioridad más alta)
                color = RED;
            else if (esPar(val)) // - Par no primo: azul
                color = BLUE;
            else // - Impar no primo: naranja
                color = ORANGE;

            DrawRectangleRec(box, color);                                   // - Celda con color
            texto(TextFormat("%d", val), box.x + 8, box.y + 14, 18, BLACK); // - Número dentro de la celda
        }
        else
        {
            DrawRectangleLinesEx(box, 2, GRAY); // - Celda vacía: solo contorno gris
        }
    }

    // --------------------------------------------------
    // Leyenda de colores
    // --------------------------------------------------
    DrawRectangle(100, 225, 16, 16, RED);
    texto("Primo", 122, 224, 14, BLACK); // - Leyenda primo
    DrawRectangle(195, 225, 16, 16, BLUE);
    texto("Par", 217, 224, 14, BLACK); // - Leyenda par
    DrawRectangle(270, 225, 16, 16, ORANGE);
    texto("Impar", 292, 224, 14, BLACK); // - Leyenda impar

    // --------------------------------------------------
    // Estado del productor
    // --------------------------------------------------
    Color colorProd = prod->isBlocked() ? RED : DARKGREEN;                // - Color según estado
    texto("Productor:", 100, 265, 18, BLACK);                             // - Etiqueta
    texto(prod->isBlocked() ? "BLOQUEADO (buffer lleno)" : "PRODUCIENDO", // - Estado textual
          220, 265, 18, colorProd);

    // --------------------------------------------------
    // Ocupación del buffer
    // --------------------------------------------------
    texto(TextFormat("Buffer: %d / %d", (int)snapshot.size(), maxSize),
          100, 293, 18, BLACK); // - Ocupación actual del buffer

    // --------------------------------------------------
    // Estado y sumas de cada consumidor
    // --------------------------------------------------
    auto &consumidores = controller.getConsumidores(); // - Lista de consumidores
    int offsetY = 335;                                 // - Posición Y inicial

    for (auto &c : consumidores) // - Iteramos sobre cada consumidor
    {
        Color colorC = c->isBlocked() ? GRAY : DARKGREEN;                           // - Gris si bloqueado
        texto(TextFormat("%-8s", c->getNombre().c_str()), 100, offsetY, 18, BLACK); // - Nombre
        texto(c->isBlocked() ? "ESPERANDO" : "ACTIVO", 230, offsetY, 18, colorC);   // - Estado
        texto(TextFormat("Suma: %d", c->getSuma()), 375, offsetY, 18, BLACK);       // - Suma acumulada
        offsetY += 32;                                                              // - Bajamos para el siguiente consumidor
    }
}