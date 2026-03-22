#include "PCScene.hpp"                // - Incluimos el header de PCScene con sus declaraciones
#include "../app/Application.hpp"     // - Incluimos Application para poder interactuar con la app
#include "raylib.h"                   // - Incluimos raylib para funciones gráficas y de tiempo
#include "../utils/tinyfiledialogs.h" // - Incluimos tinyfiledialogs para el diálogo de archivo nativo
#include "MenuScene.hpp"              // - Incluimos MenuScene para poder navegar de vuelta al menú
#include <cmath>                      // - Incluimos cmath para sin() en las animaciones de pulso

// --------------------------------------------------
// Constructor
// --------------------------------------------------
PCScene::PCScene(Application *app)
    : controller(10) // - Inicializamos el controlador solo con el tamaño del buffer, sin archivo aún
{
    this->app = app;    // - Guardamos el puntero a la aplicación en el atributo de instancia
    this->timer = 0.0f; // - Inicializamos el acumulador de tiempo en cero

    // - Inicializamos el estado de la animación de flecha
    arrowTimer = 0.0f;    // - Sin animación activa al inicio
    arrowDuration = 0.4f; // - La flecha dura 0.4 segundos por evento
    arrowIndex = -1;      // - Sin celda activa al inicio
    arrowInsert = true;   // - Valor inicial (se sobreescribe en cada evento)

    // - Inicializamos el estado del flash del buffer
    bufferFlashTimer = 0.0f; // - Sin flash activo al inicio
    bufferWasFull = false;   // - El buffer inicia sin estar lleno
    bufferWasEmpty = true;   // - El buffer inicia vacío

    fuente = LoadFont("assets/fonts/JetBrainsMono-VariableFont_wght.ttf"); // - Cargamos la fuente JetBrains Mono
    SetTextureFilter(fuente.texture, TEXTURE_FILTER_BILINEAR);             // - Aplicamos filtro bilineal para suavizado
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
// Dibuja la flecha animada sobre una celda del buffer
// --------------------------------------------------
void PCScene::drawArrow(int cellIndex, bool insert, float progress,
                        int startX, int startY, int boxSize) const
{
    float cellX = startX + cellIndex * (boxSize + 10) + boxSize / 2.0f; // - Centro X de la celda
    float arrowSize = 14.0f;                                            // - Tamaño de la cabeza
    Color color = insert ? DARKGREEN : MAROON;                          // - Verde inserción, rojo extracción

    float baseY = insert
                      ? startY - 50 + progress * 30  // - Inserción: empieza arriba y baja hacia la celda
                      : startY - 10 - progress * 30; // - Extracción: empieza en la celda y sube

    if (insert) // - Flecha apuntando hacia abajo (inserción ↓)
    {
        DrawLineEx({cellX, baseY}, {cellX, baseY + 20}, 3, color); // - Cuerpo de la flecha
        DrawTriangle(                                              // - Cabeza triangular apuntando abajo
            {cellX, baseY + 34},                                   // - Punta inferior
            {cellX - arrowSize * 0.6f, baseY + 20},                // - Vértice izquierdo
            {cellX + arrowSize * 0.6f, baseY + 20},                // - Vértice derecho
            color);
    }
    else // - Flecha apuntando hacia arriba (extracción ↑)
    {
        DrawLineEx({cellX, baseY + 20}, {cellX, baseY}, 3, color); // - Cuerpo de la flecha
        DrawTriangle(                                              // - Cabeza triangular apuntando arriba
            {cellX - arrowSize * 0.6f, baseY + 14},                // - Vértice izquierdo
            {cellX + arrowSize * 0.6f, baseY + 14},                // - Vértice derecho
            {cellX, baseY},                                        // - Punta superior
            color);
    }
}

// --------------------------------------------------
// Abre el diálogo nativo del sistema para seleccionar un .txt
// --------------------------------------------------
std::string PCScene::abrirDialogoArchivo()
{
    const char *filtros[] = {"*.txt"}; // - Filtro: solo archivos .txt

    const char *resultado = tinyfd_openFileDialog(
        "Seleccionar archivo de numeros", // - Título del diálogo
        "",                               // - Ruta inicial vacía (carpeta actual)
        1,                                // - Número de filtros
        filtros,                          // - Array de filtros
        "Archivos de texto (*.txt)",      // - Descripción del filtro
        0                                 // - 0 = selección de un solo archivo
    );

    if (resultado)
        return std::string(resultado); // - Retornamos la ruta completa como string

    return ""; // - Si el usuario canceló, retornamos cadena vacía
}

// --------------------------------------------------
// Update: lógica, input y detección de eventos de animación
// --------------------------------------------------
void PCScene::update()
{
    float dt = GetFrameTime(); // - Delta time del frame actual
    timer += dt;               // - Acumulamos tiempo total para animaciones sinusoidales

    // --------------------------------------------------
    // Detectar click en botón de menú
    // --------------------------------------------------
    Rectangle btnMenu = {10, 10, 130, 38}; // - Rectángulo del botón de volver al menú
    Vector2 mouse = GetMousePosition();    // - Posición actual del cursor

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mouse, btnMenu)) // - Click en botón de menú
    {
        if (controller.tieneArchivo())
            controller.getBuffer()->shutdown(); // - Despertamos hilos bloqueados para que terminen limpiamente
        app->setScene(new MenuScene(app));      // - Guardamos MenuScene como escena pendiente
        return;                                 // - Salimos para no procesar más input este frame
    }

    // --------------------------------------------------
    // Detectar click en botón de carga
    // --------------------------------------------------
    Rectangle btnCargar = {412, 620, 200, 45}; // - Rectángulo del botón de cargar archivo

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(mouse, btnCargar))
    {
        std::string ruta = abrirDialogoArchivo(); // - Abrimos el diálogo de selección

        if (!ruta.empty())
        {
            archivoActual = ruta;           // - Guardamos la ruta para mostrarla en pantalla
            controller.cargarArchivo(ruta); // - Iniciamos el productor con el archivo seleccionado
        }
    }

    if (!controller.tieneArchivo()) // - Sin archivo no hay eventos de animación que procesar
        return;

    Buffer *buf = controller.getBuffer(); // - Obtenemos puntero al buffer

    // --------------------------------------------------
    // Detectar evento de inserción → disparar flecha ↓
    // --------------------------------------------------
    int insertIdx = buf->lastInsertIndex.exchange(-1); // - Leemos y limpiamos el evento atómicamente
    if (insertIdx >= 0)
    {
        arrowIndex = insertIdx;     // - Celda donde se insertó el elemento
        arrowInsert = true;         // - Flecha de inserción hacia abajo
        arrowTimer = arrowDuration; // - Iniciamos el temporizador de la animación
    }

    // --------------------------------------------------
    // Detectar evento de extracción → disparar flecha ↑
    // --------------------------------------------------
    int removeIdx = buf->lastRemoveIndex.exchange(-1); // - Leemos y limpiamos el evento atómicamente
    if (removeIdx >= 0 && arrowTimer <= 0.0f)          // - Solo si no hay otra animación activa
    {
        arrowIndex = removeIdx;     // - Celda desde donde se extrajo el elemento
        arrowInsert = false;        // - Flecha de extracción hacia arriba
        arrowTimer = arrowDuration; // - Iniciamos el temporizador de la animación
    }

    // --------------------------------------------------
    // Actualizar temporizador de la flecha
    // --------------------------------------------------
    if (arrowTimer > 0.0f)
        arrowTimer -= dt; // - Decrementamos el temporizador cada frame

    // --------------------------------------------------
    // Detectar cambio de estado del buffer → activar flash
    // --------------------------------------------------
    bool nowFull = buf->isFull;   // - Estado actual de lleno
    bool nowEmpty = buf->isEmpty; // - Estado actual de vacío

    if ((nowFull != bufferWasFull) || (nowEmpty != bufferWasEmpty)) // - Si hubo cambio de estado
        bufferFlashTimer = 0.5f;                                    // - Activamos el flash por 0.5 segundos

    bufferWasFull = nowFull; // - Guardamos estado para comparar en el próximo frame
    bufferWasEmpty = nowEmpty;

    if (bufferFlashTimer > 0.0f)
        bufferFlashTimer -= dt; // - Decrementamos el flash timer cada frame
}

// --------------------------------------------------
// Draw: renderizado completo de la escena
// --------------------------------------------------
void PCScene::draw()
{

    // --------------------------------------------------
    // Botón volver al menú (esquina superior izquierda)
    // --------------------------------------------------
    Rectangle btnMenu = {10, 10, 130, 38};
    Vector2 mouseDraw = GetMousePosition();
    Color colorMenu = CheckCollisionPointRec(mouseDraw, btnMenu) ? DARKGRAY : LIGHTGRAY;
    DrawRectangleRec(btnMenu, colorMenu);
    DrawRectangleLinesEx(btnMenu, 2, DARKGRAY);
    texto("< Menu", 22, 18, 18, BLACK); // - Texto del botón

    // --------------------------------------------------
    // Título
    // --------------------------------------------------
    texto("PRODUCTOR - CONSUMIDOR", 330, 30, 30, BLACK); // - Título principal de la escena

    // --------------------------------------------------
    // Botón para cargar archivo
    // --------------------------------------------------
    Rectangle btnCargar = {412, 620, 200, 45};
    Vector2 mouse = GetMousePosition();
    Color colorBtn = CheckCollisionPointRec(mouse, btnCargar) ? DARKGRAY : LIGHTGRAY;

    DrawRectangleRec(btnCargar, colorBtn);        // - Fondo del botón
    DrawRectangleLinesEx(btnCargar, 2, DARKGRAY); // - Borde del botón
    texto("Cargar archivo", 440, 632, 18, BLACK); // - Texto del botón

    // --------------------------------------------------
    // Nombre del archivo cargado
    // --------------------------------------------------
    if (archivoActual.empty())
    {
        texto("Sin archivo cargado", 390, 672, 15, GRAY); // - Sin archivo
    }
    else
    {
        std::string nombre = archivoActual.substr(archivoActual.find_last_of("\\/") + 1);
        texto(TextFormat("Archivo: %s", nombre.c_str()), 280, 672, 15, DARKGREEN); // - Nombre del archivo
    }

    // --------------------------------------------------
    // Sin archivo: instrucción y salida anticipada
    // --------------------------------------------------
    if (!controller.tieneArchivo())
    {
        texto("Carga un archivo .txt con numeros separados por comas", 180, 360, 20, BLACK);
        return;
    }

    // --------------------------------------------------
    // Datos del buffer
    // --------------------------------------------------
    Buffer *buf = controller.getBuffer();
    std::vector<int> snapshot = buf->snapshot();
    int maxSize = buf->getMaxSize();
    Productor *prod = controller.getProductor();
    bool enSeccionCritica = buf->criticalSection; // - ¿Hay un hilo en sección crítica?

    int startX = 112; // - Posición X inicial de las celdas (centrado en 1024)
    int startY = 180; // - Posición Y inicial de las celdas
    int boxSize = 55; // - Tamaño de cada celda en píxeles

    // --------------------------------------------------
    // Flash de borde al cambiar estado del buffer
    // --------------------------------------------------
    if (bufferFlashTimer > 0.0f)
    {
        float alpha = bufferFlashTimer / 0.5f;
        Color flash = buf->isFull
                          ? Color{220, 50, 50, (unsigned char)(200 * alpha)}  // - Rojo si lleno
                          : Color{50, 200, 50, (unsigned char)(200 * alpha)}; // - Verde si se vació
        int totalW = maxSize * (boxSize + 10) - 10;
        DrawRectangle(startX - 4, startY - 4, totalW + 8, boxSize + 8, flash); // - Flash de fondo
    }

    // --------------------------------------------------
    // Dibujar celdas del buffer
    // --------------------------------------------------
    for (int i = 0; i < maxSize; i++)
    {
        Rectangle box = {
            (float)(startX + i * (boxSize + 10)), // - Posición X con espaciado entre celdas
            (float)startY,                        // - Posición Y fija
            (float)boxSize,                       // - Ancho de la celda
            (float)boxSize                        // - Alto de la celda
        };

        if (i < (int)snapshot.size()) // - Celda ocupada: dibujamos con color según tipo
        {
            int val = snapshot[i];
            Color color = GREEN;

            if (esPrimo(val))
                color = RED; // - Primo: rojo
            else if (esPar(val))
                color = BLUE; // - Par no primo: azul
            else
                color = ORANGE; // - Impar no primo: naranja

            // - Borde amarillo pulsante en la celda activa durante sección crítica
            if (enSeccionCritica && i == (int)snapshot.size() - 1)
            {
                float pulse = (float)(0.5 + 0.5 * sin(timer * 10.0));
                Color borderColor = {255, 255, 0, (unsigned char)(180 + 75 * pulse)};
                DrawRectangle(box.x - 3, box.y - 3,
                              box.width + 6, box.height + 6, borderColor);
            }

            DrawRectangleRec(box, color);                                   // - Celda con color
            texto(TextFormat("%d", val), box.x + 8, box.y + 14, 18, BLACK); // - Número dentro
        }
        else // - Celda vacía: solo contorno gris
        {
            DrawRectangleLinesEx(box, 2, GRAY); // - Contorno gris sin relleno
        }

        // --------------------------------------------------
        // Flecha animada sobre la celda activa
        // --------------------------------------------------
        if (arrowTimer > 0.0f && arrowIndex == i)
        {
            float progress = 1.0f - (arrowTimer / arrowDuration); // - 0.0 = inicio, 1.0 = fin
            drawArrow(i, arrowInsert, progress, startX, startY, boxSize);
        }
    }

    // --------------------------------------------------
    // Leyenda de colores y sección crítica
    // --------------------------------------------------
    DrawRectangle(112, 255, 16, 16, RED);
    texto("Primo", 134, 254, 14, BLACK);
    DrawRectangle(230, 255, 16, 16, BLUE);
    texto("Par", 252, 254, 14, BLACK);
    DrawRectangle(320, 255, 16, 16, ORANGE);
    texto("Impar", 342, 254, 14, BLACK);
    DrawRectangle(415, 255, 16, 16, YELLOW);
    texto("Seccion critica", 437, 254, 14, BLACK);

    // --------------------------------------------------
    // Indicador de mutex / sección crítica activa
    // --------------------------------------------------
    if (enSeccionCritica)
    {
        float pulse = (float)(0.5 + 0.5 * sin(timer * 8.0));
        Color sc = {255, 220, 0, (unsigned char)(180 + 75 * pulse)};
        DrawRectangle(700, 255, 14, 14, sc);           // - Indicador activo pulsante
        texto("MUTEX ACTIVO", 720, 253, 14, DARKGRAY); // - Etiqueta activa
    }
    else
    {
        DrawRectangle(700, 255, 14, 14, LIGHTGRAY);    // - Indicador apagado
        texto("MUTEX LIBRE", 720, 253, 14, LIGHTGRAY); // - Etiqueta apagada
    }

    // --------------------------------------------------
    // Estado del productor con ícono de color
    // --------------------------------------------------
    bool prodBloqueado = prod->isBlocked();
    Color colorProd = prodBloqueado ? RED : DARKGREEN;
    DrawRectangle(112, 300, 16, 16, colorProd);                                                 // - Ícono de estado
    texto("Productor:", 134, 298, 20, BLACK);                                                   // - Etiqueta
    texto(prodBloqueado ? "BLOQUEADO (buffer lleno)" : "PRODUCIENDO", 265, 298, 20, colorProd); // - Estado

    // --------------------------------------------------
    // Ocupación del buffer con barra visual
    // --------------------------------------------------
    texto(TextFormat("Buffer: %d / %d", (int)snapshot.size(), maxSize), 112, 335, 20, BLACK);

    int barW = 300;
    int fillW = maxSize > 0 ? ((int)snapshot.size() * barW) / maxSize : 0;
    Color fillColor = buf->isFull ? RED : DARKGREEN;
    DrawRectangle(310, 338, barW, 16, LIGHTGRAY);                   // - Fondo de la barra
    DrawRectangle(310, 338, fillW, 16, fillColor);                  // - Relleno proporcional
    DrawRectangleLinesEx({310, 338, (float)barW, 16}, 1, DARKGRAY); // - Borde de la barra

    // --------------------------------------------------
    // Estado y sumas de cada consumidor con ícono
    // --------------------------------------------------
    auto &consumidores = controller.getConsumidores();
    int offsetY = 380;

    for (auto &c : consumidores)
    {
        bool bloqueado = c->isBlocked();
        Color colorC = bloqueado ? GRAY : DARKGREEN;

        DrawRectangle(112, offsetY + 3, 16, 16, colorC);                            // - Ícono de estado
        texto(TextFormat("%-8s", c->getNombre().c_str()), 134, offsetY, 20, BLACK); // - Nombre
        texto(bloqueado ? "ESPERANDO" : "ACTIVO", 295, offsetY, 20, colorC);        // - Estado
        texto(TextFormat("Suma: %d", c->getSuma()), 490, offsetY, 20, BLACK);       // - Suma acumulada
        offsetY += 36;                                                              // - Bajamos para el siguiente consumidor
    }
}