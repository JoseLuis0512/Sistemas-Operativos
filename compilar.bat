@echo off
C:\raylib\w64devkit\bin\g++.exe -std=c++17 -o build/ProyectoSO.exe src/app/Application.cpp src/scenes/MenuScene.cpp src/scenes/PCScene.cpp src/scenes/FilosofosScene.cpp src/main.cpp -I C:/raylib/raylib/src -L C:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
if %errorlevel% == 0 (
    echo Compilacion exitosa
    build\ProyectoSO.exe
) else (
    echo Error de compilacion
)
pause