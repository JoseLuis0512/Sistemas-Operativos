#ifndef PC_SCENE_HPP // - Evitamos que este archivo se incluya más de una vez
#define PC_SCENE_HPP // - Definimos el macro que indica que ya fue incluido

#include "Scene.hpp"              // - Incluimos la clase base Scene de la que PCScene heredará
#include "../pc/PCController.hpp" // - Incluimos el controlador que gestiona productor, consumidores y buffer
#include "raylib.h"               // - Incluimos raylib para el tipo Font
#include <string>                 // - Incluimos string para guardar la ruta del archivo cargado

class Application; // - Forward declaration: le decimos al compilador que existirá la clase Application

class PCScene : public Scene
{
private:
    Application *app;          // - Puntero a la aplicación, permite que la escena interactúe con ella
    PCController controller;   // - Controlador que encapsula toda la lógica de productor y consumidores
    float timer;               // - Acumula el tiempo transcurrido frame a frame para animaciones
    std::string archivoActual; // - Ruta del archivo actualmente cargado (vacía si no hay ninguno)
    Font fuente;               // - Fuente personalizada cargada desde archivo .ttf

    // - Estado de animación de flecha (inserción / extracción)
    float arrowTimer;    // - Temporizador de la animación (0 = sin animación activa)
    float arrowDuration; // - Duración total de la animación en segundos
    int arrowIndex;      // - Índice de la celda sobre la que aparece la flecha
    bool arrowInsert;    // - true = flecha de inserción (↓), false = flecha de extracción (↑)

    // - Estado de flash del buffer al cambiar de estado
    float bufferFlashTimer; // - Temporizador del flash de borde (0 = sin flash)
    bool bufferWasFull;     // - Estado de lleno en el frame anterior para detectar cambios
    bool bufferWasEmpty;    // - Estado de vacío en el frame anterior para detectar cambios

    std::string abrirDialogoArchivo(); // - Abre el diálogo del sistema para seleccionar un archivo .txt

    // - Wrapper de DrawTextEx para simplificar el uso de la fuente en toda la escena
    void texto(const char *txt, float x, float y, float size, Color color) const;

    // - Dibuja la flecha animada sobre una celda del buffer
    void drawArrow(int cellIndex, bool insert, float progress, int startX, int startY, int boxSize) const;

public:
    PCScene(Application *app); // - Constructor: recibe un puntero a la aplicación
    ~PCScene();                // - Destructor: libera la fuente cargada en memoria

    void update() override; // - Lógica, input y detección de eventos de animación
    void draw() override;   // - Renderizado completo de la escena
};

#endif // - Fin del bloque condicional que evita doble inclusión