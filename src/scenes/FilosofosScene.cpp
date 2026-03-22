#include "FilosofosScene.hpp"     // - Incluimos el header de FilosofosScene
#include "../app/Application.hpp" // - Incluimos Application para cambiar escenas
#include "raylib.h"               // - Incluimos raylib para gráficos y tiempo
#include <cmath>                  // - Incluimos cmath para sin(), cos() y las posiciones circulares

// --------------------------------------------------
// Constructor
// --------------------------------------------------
FilosofosScene::FilosofosScene(Application *app)
    : controller(5) // - Inicializamos la mesa con 5 filósofos
{
    this->app = app;    // - Guardamos el puntero a la aplicación
    this->timer = 0.0f; // - Inicializamos el acumulador de tiempo

    fuente = LoadFont("assets/fonts/JetBrainsMono-VariableFont_wght.ttf"); // - Cargamos la fuente
    SetTextureFilter(fuente.texture, TEXTURE_FILTER_BILINEAR);             // - Filtro bilineal para suavizado
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
FilosofosScene::~FilosofosScene()
{
    UnloadFont(fuente); // - Liberamos la fuente de la memoria de la GPU
}

// --------------------------------------------------
// Wrapper de texto con fuente personalizada
// --------------------------------------------------
void FilosofosScene::texto(const char *txt, float x, float y, float size, Color color) const
{
    DrawTextEx(fuente, txt, {x, y}, size, size * 0.05f, color); // - Dibujamos con fuente y espaciado proporcional
}

// --------------------------------------------------
// Dibuja un filósofo en su posición circular
// --------------------------------------------------
void FilosofosScene::drawFilosofo(int i, int n, float cx, float cy,
                                  float radio, const Filosofo &f) const
{
    float angulo = (2.0f * PI * i / n) - PI / 2.0f; // - Ángulo del filósofo i en el círculo
    float fx = cx + radio * cosf(angulo);           // - Posición X del filósofo
    float fy = cy + radio * sinf(angulo);           // - Posición Y del filósofo

    float radioCirculo = 28.0f; // - Radio del círculo que representa al filósofo

    // - Color según estado del filósofo
    Color colorFilosofo;
    EstadoFilosofo estado = f.estado.load();

    if (estado == EstadoFilosofo::COMIENDO)
        colorFilosofo = DARKGREEN; // - Verde oscuro: comiendo
    else if (estado == EstadoFilosofo::ESPERANDO)
    {
        if (f.bloqueado)
            colorFilosofo = RED; // - Rojo: bloqueado (muchos intentos fallidos)
        else
        {
            // - Naranja pulsante mientras espera tenedores
            float pulse = (float)(0.5 + 0.5 * sin(timer * 6.0 + i));
            unsigned char r = (unsigned char)(230);
            unsigned char g = (unsigned char)(120 + 80 * pulse);
            colorFilosofo = {r, g, 0, 255};
        }
    }
    else
        colorFilosofo = SKYBLUE; // - Azul cielo: pensando

    // - Borde pulsante amarillo si está intentando adquirir tenedores (sección crítica)
    if (f.intentando)
    {
        float pulse = (float)(0.5 + 0.5 * sin(timer * 10.0 + i * 1.5));
        Color borderColor = {255, 220, 0, (unsigned char)(180 + 75 * pulse)};
        DrawCircle((int)fx, (int)fy, radioCirculo + 5, borderColor); // - Borde amarillo pulsante
    }

    DrawCircle((int)fx, (int)fy, radioCirculo, colorFilosofo); // - Círculo del filósofo
    DrawCircleLines((int)fx, (int)fy, radioCirculo, DARKGRAY); // - Borde oscuro

    // - Inicial del nombre centrada en el círculo
    std::string inicial = f.getNombre().substr(0, 1); // - Primera letra del nombre
    float tw = MeasureTextEx(fuente, inicial.c_str(), 18, 1).x;
    texto(inicial.c_str(), fx - tw / 2, fy - 9, 18, WHITE); // - Inicial centrada

    // - Nombre y estado debajo del círculo
    float offsetY = fy + radioCirculo + 5;
    texto(f.getNombre().c_str(), fx - 35, offsetY, 11, BLACK); // - Nombre

    const char *estadoTxt;
    Color estadoColor;
    if (estado == EstadoFilosofo::PENSANDO)
    {
        estadoTxt = "Pensando";
        estadoColor = DARKBLUE;
    }
    else if (estado == EstadoFilosofo::ESPERANDO)
    {
        estadoTxt = f.bloqueado ? "BLOQUEADO" : "Esperando";
        estadoColor = f.bloqueado ? RED : ORANGE;
    }
    else
    {
        estadoTxt = "Comiendo";
        estadoColor = DARKGREEN;
    }

    float sw = MeasureTextEx(fuente, estadoTxt, 11, 0.5f).x;
    texto(estadoTxt, fx - sw / 2, offsetY + 13, 11, estadoColor); // - Estado centrado

    // - Veces que comió
    texto(TextFormat("Come: %d", f.getVecesComio()), fx - 22, offsetY + 26, 10, GRAY); // - Contador
}

// --------------------------------------------------
// Dibuja un tenedor entre dos filósofos
// --------------------------------------------------
void FilosofosScene::drawTenedor(int i, int n, float cx, float cy,
                                 float radio, const Tenedor &t) const
{
    // - El tenedor i está entre el filósofo i y el filósofo (i+1)%n
    float angulo = (2.0f * PI * i / n) - PI / 2.0f + (PI / n); // - Ángulo medio entre dos filósofos
    float tx = cx + (radio * 0.55f) * cosf(angulo);            // - Posición X del tenedor
    float ty = cy + (radio * 0.55f) * sinf(angulo);            // - Posición Y del tenedor

    Color colorTenedor = t.enUso ? RED : DARKGRAY; // - Rojo si en uso, gris si libre

    // - Dibujamos el tenedor como un rectángulo pequeño rotado
    DrawRectanglePro(
        {tx, ty, 8, 22},                // - Rectángulo del tenedor
        {4, 11},                        // - Pivote en el centro
        angulo * (180.0f / PI) + 90.0f, // - Rotación según el ángulo
        colorTenedor);                  // - Color según estado

    // - Número del tenedor
    texto(TextFormat("%d", t.getId()),
          tx - 4, ty - 5, 10,
          t.enUso ? WHITE : LIGHTGRAY); // - Blanco si en uso, gris si libre
}

// --------------------------------------------------
// Update: input y lógica
// --------------------------------------------------
void FilosofosScene::update()
{
    timer += GetFrameTime(); // - Acumulamos tiempo para animaciones

    // - (Sin botón de regreso al menú en esta escena)
}

// --------------------------------------------------
// Draw: renderizado completo de la escena
// --------------------------------------------------
void FilosofosScene::draw()
{
    // --------------------------------------------------
    // Título
    // --------------------------------------------------
    texto("FILOSOFOS COMENSALES", 240, 40, 28, BLACK); // - Título de la escena

    // --------------------------------------------------
    // Mesa circular
    // --------------------------------------------------
    float cx = 400.0f;    // - Centro X de la mesa
    float cy = 300.0f;    // - Centro Y de la mesa
    float radio = 160.0f; // - Radio del círculo de filósofos

    // - Mesa (círculo central)
    DrawCircle((int)cx, (int)cy, 70, {180, 120, 60, 255});     // - Mesa de madera (café)
    DrawCircleLines((int)cx, (int)cy, 70, {120, 80, 40, 255}); // - Borde de la mesa
    texto("MESA", cx - 18, cy - 8, 14, WHITE);                 // - Texto en la mesa

    Mesa &mesa = controller.getMesa(); // - Obtenemos referencia a la mesa
    int n = mesa.getN();               // - Número de filósofos

    // - Dibujamos los tenedores primero (quedan debajo de los filósofos)
    for (int i = 0; i < n; i++)
        drawTenedor(i, n, cx, cy, radio, *mesa.getTenedores()[i]);

    // - Dibujamos los filósofos encima
    for (int i = 0; i < n; i++)
        drawFilosofo(i, n, cx, cy, radio, *mesa.getFilosofos()[i]);

    // --------------------------------------------------
    // Panel lateral: estado de cada filósofo
    // --------------------------------------------------
    texto("Estado actual:", 620, 100, 16, BLACK); // - Encabezado del panel

    int offsetY = 125;
    for (int i = 0; i < n; i++)
    {
        const Filosofo &f = *mesa.getFilosofos()[i];
        EstadoFilosofo estado = f.estado.load();
        const char *eTxt;
        Color eColor;

        if (estado == EstadoFilosofo::PENSANDO)
        {
            eTxt = "Pensando";
            eColor = DARKBLUE;
        }
        else if (estado == EstadoFilosofo::ESPERANDO)
        {
            eTxt = f.bloqueado ? "BLOQUEADO" : "Esperando";
            eColor = f.bloqueado ? RED : ORANGE;
        }
        else
        {
            eTxt = "Comiendo";
            eColor = DARKGREEN;
        }

        DrawRectangle(620, offsetY, 12, 12, eColor);                                             // - Ícono de color
        texto(TextFormat("%-11s %s", f.getNombre().c_str(), eTxt), 638, offsetY - 1, 12, BLACK); // - Nombre y estado
        offsetY += 18;
    }

    // --------------------------------------------------
    // Panel lateral: estado de cada tenedor
    // --------------------------------------------------
    texto("Tenedores:", 620, offsetY + 10, 16, BLACK);
    offsetY += 30;

    for (int i = 0; i < n; i++)
    {
        const Tenedor &t = *mesa.getTenedores()[i];
        Color color = t.enUso ? RED : DARKGRAY;
        const char *tTxt = t.enUso
                               ? TextFormat("T%d - Fil.%d", t.getId(), (int)t.usadoPor)
                               : TextFormat("T%d - Libre", t.getId());

        DrawRectangle(620, offsetY, 12, 12, color); // - Ícono de color
        texto(tTxt, 638, offsetY - 1, 12, BLACK);   // - Estado del tenedor
        offsetY += 18;
    }

    // --------------------------------------------------
    // Leyenda
    // --------------------------------------------------
    offsetY += 15;
    texto("Leyenda:", 620, offsetY, 14, BLACK);
    offsetY += 18;
    DrawRectangle(620, offsetY, 12, 12, SKYBLUE);
    texto("Pensando", 638, offsetY - 1, 12, BLACK);
    offsetY += 16;
    DrawRectangle(620, offsetY, 12, 12, ORANGE);
    texto("Esperando", 638, offsetY - 1, 12, BLACK);
    offsetY += 16;
    DrawRectangle(620, offsetY, 12, 12, DARKGREEN);
    texto("Comiendo", 638, offsetY - 1, 12, BLACK);
    offsetY += 16;
    DrawRectangle(620, offsetY, 12, 12, RED);
    texto("Bloqueado", 638, offsetY - 1, 12, BLACK);
    offsetY += 16;
    DrawRectangle(620, offsetY, 12, 12, YELLOW);
    texto("Secc. critica", 638, offsetY - 1, 12, BLACK);
}