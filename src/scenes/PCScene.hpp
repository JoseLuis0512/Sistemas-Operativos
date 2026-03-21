#ifndef PC_SCENE_HPP // - Evitamos que este archivo se incluya más de una vez
#define PC_SCENE_HPP // - Definimos el macro que indica que ya fue incluido

#include "Scene.hpp"              // - Incluimos la clase base Scene de la que PCScene heredará
#include "../pc/PCController.hpp" // - Incluimos el controlador que gestiona productor, consumidores y buffer
#include "raylib.h"               // - Incluimos raylib para el tipo Font
#include <string>                 // - Incluimos string para guardar la ruta del archivo cargado

class Application; // - Forward declaration: le decimos al compilador que existirá la clase Application

class PCScene : public Scene // - Definimos la clase PCScene que hereda de Scene
{
private:
    Application *app;          // - Puntero a la aplicación, permite que la escena interactúe con ella
    PCController controller;   // - Controlador que encapsula toda la lógica de productor y consumidores
    float timer;               // - Acumula el tiempo transcurrido frame a frame para animaciones
    std::string archivoActual; // - Ruta del archivo actualmente cargado (vacía si no hay ninguno)
    Font fuente;               // - Fuente personalizada cargada desde archivo .ttf

    std::string abrirDialogoArchivo(); // - Abre el diálogo del sistema para seleccionar un archivo .txt

    // - Wrapper de DrawTextEx para simplificar el uso de la fuente en toda la escena
    void texto(const char *txt, float x, float y, float size, Color color) const;

public:
    PCScene(Application *app); // - Constructor: recibe un puntero a la aplicación
    ~PCScene();                // - Destructor: libera la fuente cargada en memoria

    void update() override; // - Método obligatorio de Scene: ejecuta la lógica de la escena
    void draw() override;   // - Método obligatorio de Scene: renderiza la escena
};

#endif // - Fin del bloque condicional que evita doble inclusión