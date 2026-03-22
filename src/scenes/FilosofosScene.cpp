#include "FilosofosScene.hpp"     // - Incluimos el header de FilosofosScene
#include "../app/Application.hpp" // - Incluimos Application para cambiar escenas
#include "raylib.h"               // - Incluimos raylib para gráficos y tiempo
#include <cmath>                  // - Incluimos cmath para sin(), cos() en posiciones circulares
#include <climits>                // - Incluimos climits para INT_MAX en la búsqueda de mínimos

// --------------------------------------------------
// Constructor
// --------------------------------------------------
FilosofosScene::FilosofosScene(Application *app)
{
    this->app = app;
    this->timer = 0.0f;
    this->estadoEscena = EstadoEscena::CONFIGURANDO;
    this->nFilosofos = 5;
    this->duracionSeg = 30;
    this->tiempoRestante = 0.0f;
    this->tiempoTotal = 0.0f;
    this->controller = nullptr;

    fuente = LoadFont("assets/fonts/JetBrainsMono-VariableFont_wght.ttf");
    SetTextureFilter(fuente.texture, TEXTURE_FILTER_BILINEAR);
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------
FilosofosScene::~FilosofosScene()
{
    UnloadFont(fuente);
}

// --------------------------------------------------
// Wrapper de texto
// --------------------------------------------------
void FilosofosScene::texto(const char *txt, float x, float y, float size, Color color) const
{
    DrawTextEx(fuente, txt, {x, y}, size, size * 0.05f, color);
}

// --------------------------------------------------
// Update
// --------------------------------------------------
void FilosofosScene::update()
{
    timer += GetFrameTime();

    if (estadoEscena == EstadoEscena::CONFIGURANDO)
    {
        Vector2 mouse = GetMousePosition();
        Rectangle btnMenosN = {340, 300, 50, 50};   // - Botón - filósofos
        Rectangle btnMasN = {520, 300, 50, 50};     // - Botón + filósofos
        Rectangle btnMenosD = {340, 400, 50, 50};   // - Botón - duración
        Rectangle btnMasD = {520, 400, 50, 50};     // - Botón + duración
        Rectangle btnIniciar = {362, 490, 300, 55}; // - Botón iniciar

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mouse, btnMenosN) && nFilosofos > 2)
                nFilosofos--;
            else if (CheckCollisionPointRec(mouse, btnMasN) && nFilosofos < 10)
                nFilosofos++;
            else if (CheckCollisionPointRec(mouse, btnMenosD) && duracionSeg > 10)
                duracionSeg -= 10;
            else if (CheckCollisionPointRec(mouse, btnMasD) && duracionSeg < 120)
                duracionSeg += 10;
            else if (CheckCollisionPointRec(mouse, btnIniciar))
            {
                controller = std::make_unique<FilosofosController>(nFilosofos);
                tiempoRestante = (float)duracionSeg;
                estadoEscena = EstadoEscena::EJECUTANDO;
            }
        }
    }
    else if (estadoEscena == EstadoEscena::EJECUTANDO)
    {
        tiempoRestante -= GetFrameTime();

        if (tiempoRestante <= 0.0f)
        {
            tiempoRestante = 0.0f;
            tiempoTotal = (float)duracionSeg;

            Mesa &mesa = controller->getMesa();
            for (int i = 0; i < mesa.getN(); i++)
                mesa.getFilosofos()[i]->running = false;

            estadoEscena = EstadoEscena::RESULTADOS;
        }
    }
    else if (estadoEscena == EstadoEscena::RESULTADOS)
    {
        Vector2 mouse = GetMousePosition();
        Rectangle btnNueva = {250, 680, 220, 50};
        Rectangle btnReiniciar = {554, 680, 220, 50};

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (CheckCollisionPointRec(mouse, btnNueva))
            {
                controller = nullptr;
                estadoEscena = EstadoEscena::CONFIGURANDO;
                timer = 0.0f;
            }
            else if (CheckCollisionPointRec(mouse, btnReiniciar))
            {
                controller = nullptr;
                estadoEscena = EstadoEscena::CONFIGURANDO;
                nFilosofos = 5;
                duracionSeg = 30;
                timer = 0.0f;
            }
        }
    }
}

// --------------------------------------------------
// Draw
// --------------------------------------------------
void FilosofosScene::draw()
{
    if (estadoEscena == EstadoEscena::CONFIGURANDO)
        drawConfiguracion();
    else if (estadoEscena == EstadoEscena::EJECUTANDO)
        drawEjecucion();
    else
        drawResultados();
}

// --------------------------------------------------
// Pantalla de configuración
// --------------------------------------------------
void FilosofosScene::drawConfiguracion()
{
    // - Título y subtítulo
    texto("FILOSOFOS COMENSALES", 290, 60, 30, BLACK);
    texto("Configura la simulacion:", 340, 110, 20, DARKGRAY);

    Vector2 mouse = GetMousePosition();

    // --------------------------------------------------
    // Fila 1: número de filósofos
    // --------------------------------------------------
    texto("Numero de filosofos:", 220, 270, 20, BLACK);

    Rectangle btnMenosN = {340, 300, 50, 50};
    DrawRectangleRec(btnMenosN, CheckCollisionPointRec(mouse, btnMenosN) ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(btnMenosN, 2, DARKGRAY);
    texto("-", 361, 312, 24, BLACK);

    texto(TextFormat("%d", nFilosofos), 408, 308, 32, BLACK);

    Rectangle btnMasN = {520, 300, 50, 50};
    DrawRectangleRec(btnMasN, CheckCollisionPointRec(mouse, btnMasN) ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(btnMasN, 2, DARKGRAY);
    texto("+", 539, 312, 24, BLACK);

    texto("(2 - 10)", 590, 315, 16, GRAY);

    // --------------------------------------------------
    // Fila 2: duración
    // --------------------------------------------------
    texto("Duracion en segundos:", 220, 370, 20, BLACK);

    Rectangle btnMenosD = {340, 400, 50, 50};
    DrawRectangleRec(btnMenosD, CheckCollisionPointRec(mouse, btnMenosD) ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(btnMenosD, 2, DARKGRAY);
    texto("-", 361, 412, 24, BLACK);

    texto(TextFormat("%ds", duracionSeg), 398, 408, 32, BLACK);

    Rectangle btnMasD = {520, 400, 50, 50};
    DrawRectangleRec(btnMasD, CheckCollisionPointRec(mouse, btnMasD) ? DARKGRAY : LIGHTGRAY);
    DrawRectangleLinesEx(btnMasD, 2, DARKGRAY);
    texto("+", 539, 412, 24, BLACK);

    texto("(10 - 120s)", 590, 415, 16, GRAY);

    // --------------------------------------------------
    // Preview circular
    // --------------------------------------------------
    float cx = 512.0f, cy = 610.0f, radio = 65.0f;
    DrawCircle((int)cx, (int)cy, 22, {180, 120, 60, 255});
    texto("Mesa", cx - 18, cy - 8, 13, WHITE);

    for (int i = 0; i < nFilosofos; i++)
    {
        float angulo = (2.0f * PI * i / nFilosofos) - PI / 2.0f;
        float fx = cx + radio * cosf(angulo);
        float fy = cy + radio * sinf(angulo);
        DrawCircle((int)fx, (int)fy, 16, SKYBLUE);
        DrawCircleLines((int)fx, (int)fy, 16, DARKGRAY);
        texto(TextFormat("%d", i), fx - 5, fy - 8, 14, WHITE);
    }

    // --------------------------------------------------
    // Botón iniciar
    // --------------------------------------------------
    Rectangle btnIniciar = {362, 490, 300, 55};
    Color cIni = CheckCollisionPointRec(mouse, btnIniciar) ? DARKGREEN : GREEN;
    DrawRectangleRec(btnIniciar, cIni);
    DrawRectangleLinesEx(btnIniciar, 2, DARKGREEN);
    texto("Iniciar simulacion", 405, 506, 22, WHITE);
}

// --------------------------------------------------
// Pantalla de ejecución
// --------------------------------------------------
void FilosofosScene::drawEjecucion()
{
    // - Título
    texto("FILOSOFOS COMENSALES", 330, 18, 26, BLACK);

    // - Barra de tiempo
    float progreso = tiempoRestante / (float)duracionSeg;
    int barW = 500;
    int fillW = (int)(barW * progreso);
    Color fillColor = progreso > 0.3f ? DARKGREEN : RED;
    DrawRectangle(262, 52, barW, 18, LIGHTGRAY);
    DrawRectangle(262, 52, fillW, 18, fillColor);
    DrawRectangleLinesEx({262, 52, (float)barW, 18}, 1, DARKGRAY);
    texto(TextFormat("%.0f s restantes", tiempoRestante), 462, 74, 16, DARKGRAY);

    // --------------------------------------------------
    // Mesa circular — centrada en la mitad izquierda
    // --------------------------------------------------
    float cx = 380.0f;    // - Centro X de la mesa
    float cy = 420.0f;    // - Centro Y de la mesa
    float radio = 200.0f; // - Radio del círculo de filósofos

    DrawCircle((int)cx, (int)cy, 75, {180, 120, 60, 255});
    DrawCircleLines((int)cx, (int)cy, 75, {120, 80, 40, 255});
    texto("MESA", cx - 22, cy - 10, 18, WHITE);

    Mesa &mesa = controller->getMesa();
    int n = mesa.getN();

    for (int i = 0; i < n; i++)
        drawTenedor(i, n, cx, cy, radio, *mesa.getTenedores()[i]);
    for (int i = 0; i < n; i++)
        drawFilosofo(i, n, cx, cy, radio, *mesa.getFilosofos()[i]);

    // --------------------------------------------------
    // Panel derecho — estado de filósofos y tenedores
    // --------------------------------------------------
    int px = 640; // - X del panel lateral
    int py = 100; // - Y inicial del panel lateral

    DrawRectangle(px - 8, py - 8, 360, 640, {245, 245, 245, 255});                    // - Fondo del panel
    DrawRectangleLinesEx({(float)(px - 8), (float)(py - 8), 360, 640}, 1, LIGHTGRAY); // - Borde del panel

    texto("Estado filosofos:", px, py, 17, BLACK);
    py += 28;

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

        DrawRectangle(px, py + 2, 14, 14, eColor); // - Ícono de color
        texto(f.getNombre().c_str(), px + 20, py, 15, BLACK);
        texto(eTxt, px + 165, py, 15, eColor);
        texto(TextFormat("x%d", f.getVecesComio()), px + 295, py, 15, GRAY);
        py += 22;
    }

    py += 12;
    DrawLine(px - 8, py, px + 352, py, LIGHTGRAY); // - Separador
    py += 12;

    texto("Estado tenedores:", px, py, 17, BLACK);
    py += 28;

    for (int i = 0; i < n; i++)
    {
        const Tenedor &t = *mesa.getTenedores()[i];
        Color col = t.enUso ? RED : DARKGRAY;
        const char *tTxt = t.enUso
                               ? TextFormat("Tenedor %d  ->  Fil. %d", t.getId(), (int)t.usadoPor)
                               : TextFormat("Tenedor %d  ->  Libre", t.getId());

        DrawRectangle(px, py + 2, 14, 14, col);
        texto(tTxt, px + 20, py, 15, BLACK);
        py += 22;
    }

    py += 12;
    DrawLine(px - 8, py, px + 352, py, LIGHTGRAY); // - Separador
    py += 12;

    // - Leyenda
    texto("Leyenda:", px, py, 16, BLACK);
    py += 24;
    DrawRectangle(px, py + 2, 14, 14, SKYBLUE);
    texto("Pensando", px + 20, py, 15, BLACK);
    py += 20;
    DrawRectangle(px, py + 2, 14, 14, ORANGE);
    texto("Esperando", px + 20, py, 15, BLACK);
    py += 20;
    DrawRectangle(px, py + 2, 14, 14, DARKGREEN);
    texto("Comiendo", px + 20, py, 15, BLACK);
    py += 20;
    DrawRectangle(px, py + 2, 14, 14, RED);
    texto("Bloqueado", px + 20, py, 15, BLACK);
    py += 20;
    DrawRectangle(px, py + 2, 14, 14, YELLOW);
    texto("Secc. critica", px + 20, py, 15, BLACK);
}

// --------------------------------------------------
// Pantalla de resultados
// --------------------------------------------------
void FilosofosScene::drawResultados()
{
    texto("RESULTADOS DE LA SIMULACION", 250, 40, 28, BLACK);
    texto(TextFormat("Duracion: %.0f segundos  |  Filosofos: %d", tiempoTotal, nFilosofos),
          250, 85, 18, DARKGRAY);

    Mesa &mesa = controller->getMesa();
    int n = mesa.getN();

    // - Buscamos max y min
    int maxComio = 0, minComio = INT_MAX, idxMax = 0, idxMin = 0;
    for (int i = 0; i < n; i++)
    {
        int v = mesa.getFilosofos()[i]->getVecesComio();
        if (v > maxComio)
        {
            maxComio = v;
            idxMax = i;
        }
        if (v < minComio)
        {
            minComio = v;
            idxMin = i;
        }
    }

    // --------------------------------------------------
    // Tabla
    // --------------------------------------------------
    int tx = 120; // - X inicial de la tabla
    int ty = 130; // - Y inicial de la tabla

    // - Encabezados
    DrawRectangle(tx, ty, 784, 28, {210, 210, 210, 255});
    texto("Filosofo", tx + 10, ty + 6, 16, DARKGRAY);
    texto("Veces comio", tx + 280, ty + 6, 16, DARKGRAY);
    texto("Int. fallidos", tx + 460, ty + 6, 16, DARKGRAY);
    texto("Resultado", tx + 630, ty + 6, 16, DARKGRAY);
    ty += 30;

    for (int i = 0; i < n; i++)
    {
        const Filosofo &f = *mesa.getFilosofos()[i];
        int comio = f.getVecesComio();
        int fallidos = (int)f.totalIntentosFallidos;

        Color colorFila = BLACK;
        const char *tag = "";
        Color tagColor = BLACK;

        if (i == idxMax)
        {
            colorFila = DARKGREEN;
            tag = "Mas comio";
            tagColor = DARKGREEN;
        }
        if (i == idxMin)
        {
            colorFila = RED;
            tag = "Menos comio";
            tagColor = RED;
        }

        // - Fondo alternado
        if (i % 2 == 0)
            DrawRectangle(tx, ty, 784, 26, {248, 248, 248, 255});

        DrawRectangle(tx + 4, ty + 5, 16, 16,
                      i == idxMax ? DARKGREEN : (i == idxMin ? RED : SKYBLUE)); // - Ícono

        texto(TextFormat("%s", f.getNombre().c_str()), tx + 26, ty + 4, 16, colorFila);
        texto(TextFormat("%d", comio), tx + 290, ty + 4, 16, colorFila);
        texto(TextFormat("%d", fallidos), tx + 470, ty + 4, 16, colorFila);
        texto(tag, tx + 635, ty + 4, 15, tagColor);

        ty += 28;
    }

    DrawLine(tx, ty + 4, tx + 784, ty + 4, DARKGRAY); // - Línea final
    ty += 20;

    // --------------------------------------------------
    // Resumen
    // --------------------------------------------------
    texto("Resumen:", tx, ty + 5, 19, BLACK);
    ty += 32;

    DrawRectangle(tx, ty, 16, 16, DARKGREEN);
    texto(TextFormat("Mas comio:   %s con %d veces",
                     mesa.getFilosofos()[idxMax]->getNombre().c_str(), maxComio),
          tx + 22, ty, 17, DARKGREEN);
    ty += 26;

    DrawRectangle(tx, ty, 16, 16, RED);
    texto(TextFormat("Menos comio: %s con %d veces",
                     mesa.getFilosofos()[idxMin]->getNombre().c_str(), minComio),
          tx + 22, ty, 17, RED);
    ty += 32;

    if (maxComio - minComio > 3)
    {
        DrawRectangle(tx - 4, ty - 4, 600, 28, {255, 220, 220, 255}); // - Fondo de advertencia
        texto("! Posible inanicion detectada (diferencia > 3 comidas)", tx + 4, ty, 15, MAROON);
        ty += 32;
    }

    // --------------------------------------------------
    // Botones
    // --------------------------------------------------
    Vector2 mouse = GetMousePosition();
    Rectangle btnNueva = {250, 680, 220, 50};
    Rectangle btnReiniciar = {554, 680, 220, 50};

    Color cNueva = CheckCollisionPointRec(mouse, btnNueva) ? DARKBLUE : BLUE;
    Color cRein = CheckCollisionPointRec(mouse, btnReiniciar) ? DARKGRAY : LIGHTGRAY;

    DrawRectangleRec(btnNueva, cNueva);
    DrawRectangleLinesEx(btnNueva, 2, DARKBLUE);
    texto("Nueva simulacion", 270, 695, 18, WHITE);

    DrawRectangleRec(btnReiniciar, cRein);
    DrawRectangleLinesEx(btnReiniciar, 2, DARKGRAY);
    texto("Reiniciar", 588, 695, 18, BLACK);
}

// --------------------------------------------------
// Dibuja un filósofo
// --------------------------------------------------
void FilosofosScene::drawFilosofo(int i, int n, float cx, float cy,
                                  float radio, const Filosofo &f) const
{
    float angulo = (2.0f * PI * i / n) - PI / 2.0f;
    float fx = cx + radio * cosf(angulo);
    float fy = cy + radio * sinf(angulo);
    float radioCirculo = 30.0f;

    EstadoFilosofo estado = f.estado.load();
    Color colorF;

    if (estado == EstadoFilosofo::COMIENDO)
        colorF = DARKGREEN;
    else if (estado == EstadoFilosofo::ESPERANDO)
    {
        if (f.bloqueado)
            colorF = RED;
        else
        {
            float p = (float)(0.5 + 0.5 * sin(timer * 6.0 + i));
            colorF = {230, (unsigned char)(120 + 80 * p), 0, 255};
        }
    }
    else
        colorF = SKYBLUE;

    if (f.intentando) // - Borde amarillo pulsante: sección crítica
    {
        float p = (float)(0.5 + 0.5 * sin(timer * 10.0 + i * 1.5));
        DrawCircle((int)fx, (int)fy, radioCirculo + 6,
                   {255, 220, 0, (unsigned char)(180 + 75 * p)});
    }

    DrawCircle((int)fx, (int)fy, radioCirculo, colorF);
    DrawCircleLines((int)fx, (int)fy, radioCirculo, DARKGRAY);

    // - Inicial centrada
    std::string ini = f.getNombre().substr(0, 1);
    float tw = MeasureTextEx(fuente, ini.c_str(), 20, 1).x;
    texto(ini.c_str(), fx - tw / 2, fy - 10, 20, WHITE);

    // - Nombre y estado debajo del círculo
    float offY = fy + radioCirculo + 6;
    float nw = MeasureTextEx(fuente, f.getNombre().c_str(), 13, 0.6f).x;
    texto(f.getNombre().c_str(), fx - nw / 2, offY, 13, BLACK);

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

    float sw = MeasureTextEx(fuente, eTxt, 13, 0.6f).x;
    texto(eTxt, fx - sw / 2, offY + 15, 13, eColor);
    texto(TextFormat("Come: %d", f.getVecesComio()), fx - 28, offY + 30, 12, GRAY);
}

// --------------------------------------------------
// Dibuja un tenedor
// --------------------------------------------------
void FilosofosScene::drawTenedor(int i, int n, float cx, float cy,
                                 float radio, const Tenedor &t) const
{
    float angulo = (2.0f * PI * i / n) - PI / 2.0f + (PI / n);
    float tx = cx + (radio * 0.52f) * cosf(angulo);
    float ty = cy + (radio * 0.52f) * sinf(angulo);

    Color color = t.enUso ? RED : DARKGRAY;
    DrawRectanglePro({tx, ty, 9, 26}, {4.5f, 13},
                     angulo * (180.0f / PI) + 90.0f, color);
    texto(TextFormat("%d", t.getId()), tx - 5, ty - 6, 12,
          t.enUso ? WHITE : LIGHTGRAY);
}