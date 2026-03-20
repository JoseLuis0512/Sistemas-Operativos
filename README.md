# Sistemas-Operativos
Proyecto del Curso Sistemas Operativos, Universidad Rafael Landívar, Primer Semestre 2026.

# Participantes
David Alconero Lepe         - 
José Luis Enríquez Barillas - 1053223
Anthony Hernánddez          -
Ana Sofía Sigüenza Aguilar  - 1333123

# Descripción General.
En este repositorio se encuentrar las soluciones implementadas a los problemas (Productor/Consumidor y Filósofos/Comensales) del enunciado del Proyecto.
El lenguaje utilizado para este proyecto es C++ y la Librería de RayLib para la interfaz gráfica.

# Estructura de Carpetas.
ProyectoSO/
│
├── README.md
├── CMakeLists.txt (opcional si usas CMake)
│
├── docs/                      
│   ├── manual_usuario.md
│   ├── manual_tecnico.md
│   ├── diagramas/
│   │   ├── productor_consumidor.png
│   │   ├── filosofos.png
│
├── data/                     
│   ├── numeros.txt            
│
├── src/                      
│
│   ├── main.cpp               
│
│   ├── core/                  
│   │   ├── ThreadUtils.hpp
│   │   ├── Logger.hpp
│
│   ├── productor_consumidor/  
│   │   ├── Buffer.hpp
│   │   ├── Buffer.cpp
│   │   ├── Productor.hpp
│   │   ├── Productor.cpp
│   │   ├── Consumidor.hpp
│   │   ├── Consumidor.cpp
│   │   ├── PCController.hpp   
│   │   ├── PCController.cpp
│
│   ├── filosofos/            
│   │   ├── Filosofo.hpp
│   │   ├── Filosofo.cpp
│   │   ├── Tenedor.hpp
│   │   ├── Tenedor.cpp
│   │   ├── Mesa.hpp
│   │   ├── Mesa.cpp
│   │   ├── FilosofosController.hpp
│   │   ├── FilosofosController.cpp
│
│   ├── ui/                    
│   │   ├── Menu.hpp
│   │   ├── Menu.cpp
│   │   ├── AnimacionPC.hpp
│   │   ├── AnimacionPC.cpp
│   │   ├── AnimacionFilosofos.hpp
│   │   ├── AnimacionFilosofos.cpp
│
├── build/                     
|
│
└── bin/                       