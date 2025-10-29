/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main(void)
{
    // --- Inicialización ---
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Juego de la Vida - Raylib base");
    SetTargetFPS(10);  // Velocidad baja para observar las generaciones

    // --- Configuración de la grilla ---
    const int cols = 80;
    const int rows = 45;
    const int cellSize = 10;

    bool grid[45][80] = { false };
    bool next[45][80] = { false };

    bool running = false;

    srand(time(NULL));

    // --- Bucle principal del juego ---
    while (!WindowShouldClose())
    {
        // --- INPUT / CONTROLES ---
        if (IsKeyPressed(KEY_SPACE)) running = !running;  // Iniciar/Pausar
        if (IsKeyPressed(KEY_C)) {                        // Limpiar grilla
            for (int y = 0; y < rows; y++)
                for (int x = 0; x < cols; x++)
                    grid[y][x] = false;
        }

        if (IsKeyPressed(KEY_R)) {                        // Aleatorizar
            for (int y = 0; y < rows; y++)
                for (int x = 0; x < cols; x++)
                    grid[y][x] = GetRandomValue(0, 4) == 0; // ~25% vivas
        }

         //Dibujar o borrar con el mouse cuando está en pausa
        if (!running && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            int x = mouse.x / cellSize;
            int y = mouse.y / cellSize;
            if (x >= 0 && x < cols && y >= 0 && y < rows)
                grid[y][x] = true;
        }
        if (!running && IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 mouse = GetMousePosition();
            int x = mouse.x / cellSize;
            int y = mouse.y / cellSize;
            if (x >= 0 && x < cols && y >= 0 && y < rows)
                grid[y][x] = false;
        }

// --- LÓGICA DEL JUEGO ---
        if (running)
        {
            {
                for (int y = 0; y < rows; y++)
                for (int x = 0; x < cols; x++)
                    grid[y][x] = GetRandomValue(0, 4) == 0;
            }
            for (int y = 0; y < rows; y++)
            {
                for (int x = 0; x < cols; x++)
                {
                    int alive = 0;

                    // Contar vecinos vivos (considerando bordes)
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        for (int dx = -1; dx <= 1; dx++)
                        {
                            if (dx == 0 && dy == 0) continue;
                            int nx = x + dx;
                            int ny = y + dy;

                            if (nx >= 0 && nx < cols && ny >= 0 && ny < rows)
                                if (grid[ny][nx]) alive++;
                        }
                    }

                    // Reglas de Conway
                    if (grid[y][x] && (alive == 2 || alive == 3))
                        next[y][x] = true;
                    else if (!grid[y][x] && alive == 3)
                        next[y][x] = true;
                    else
                        next[y][x] = false;
                }
            }

            // Actualizar estado
            for (int y = 0; y < rows; y++)
                for (int x = 0; x < cols; x++)
                    grid[y][x] = next[y][x];
        }
// --- DIBUJO ---
        BeginDrawing();
            ClearBackground(BLACK);

            // Dibujar celdas
            for (int y = 0; y < rows; y++)
            {
                for (int x = 0; x < cols; x++)
                {
                    if (grid[y][x])
                        DrawRectangle(x * cellSize, y * cellSize, cellSize - 1, cellSize - 1, BLUE);
                    else
                        DrawRectangleLines(x * cellSize, y * cellSize, cellSize, cellSize, DARKGRAY);
                }
            }

            // Mostrar texto
            DrawText("Juego de la Vida - Conway", 10, 10, 20, RAYWHITE);
            DrawText("ESPACIO: Iniciar/Pausar", 10, 35, 15, LIGHTGRAY);
            DrawText("R: Aleatorio | C: Limpiar", 10, 55, 15, LIGHTGRAY);
            DrawText("Click izq: Viva | Der: Muerta (en pausa)", 10, 75, 15, LIGHTGRAY);

            DrawText(running ? "Simulando..." : "En pausa", 10, screenHeight - 25, 20, running ? GREEN : RED);
        EndDrawing();
    }

    // --- Cierre ---
    CloseWindow();
    return 0;
}