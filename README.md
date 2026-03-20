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
├── assets/                     
│   ├── fonts/
│   ├── textures/
│
├── data/                        
│   ├── numeros.txt              
│
├── src/                         
│
│   ├── main.cpp                 
│
│   ├── app/                     
│   │   ├── Application.hpp
│   │   ├── Application.cpp
│
│   ├── scenes/                  
│   │   ├── Scene.hpp           
│   │   ├── MenuScene.hpp
│   │   ├── MenuScene.cpp
│   │   ├── PCScene.hpp
│   │   ├── PCScene.cpp
│   │   ├── FilosofosScene.hpp
│   │   ├── FilosofosScene.cpp
│
│   ├── pc/                      
│   │   ├── Buffer.hpp
│   │   ├── Productor.hpp
│   │   ├── Consumidor.hpp
│   │   ├── PCController.hpp
│
│   ├── filosofos/              
│   │   ├── Filosofo.hpp
│   │   ├── Tenedor.hpp
│   │   ├── Mesa.hpp
│   │   ├── FilosofosController.hpp
│
│   ├── utils/                   
│       ├── Colors.hpp
│       ├── Helpers.hpp
│
├── build/                       
├── bin/                        
│
└── README.md                     