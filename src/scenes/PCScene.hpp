#ifndef PC_SCENE_HPP // - Evitamos que este archivo se incluya más de una vez
#define PC_SCENE_HPP // - Definimos el macro que indica que ya fue incluido

#include "Scene.hpp"              // - Incluimos la clase base Scene de la que PCScene heredará
#include "../pc/PCController.hpp" // - Incluimos el controlador que gestiona productor, consumidores y buffer
#include <string>                 // - Incluimos string para guardar la ruta del archivo cargado

class Application; // - Forward declaration: le decimos al compilador que existirá la clase Application

class PCScene : public Scene // - Definimos la clase PCScene que hereda de Scene
{
private:
    Application *app;          // - Puntero a la aplicación, permite que la escena interactúe con ella
    PCController controller;   // - Controlador que encapsula toda la lógica de productor y consumidores
    float timer;               // - Acumula el tiempo transcurrido frame a frame para animaciones
    std::string archivoActual; // - Ruta del archivo actualmente cargado (vacía si no hay ninguno)

    std::string abrirDialogoArchivo(); // - Abre el diálogo de Windows para seleccionar un archivo .txt

public:
    PCScene(Application *app); // - Constructor: recibe un puntero a la aplicación
    ~PCScene() = default;      // - Destructor: PCController limpia sus propios recursos automáticamente

    void update() override; // - Método obligatorio de Scene: ejecuta la lógica de la escena
    void draw() override;   // - Método obligatorio de Scene: renderiza la escena
};

#endif // - Fin del bloque condicional que evita doble inclusión