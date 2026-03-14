#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#include "./src/boid_manager.h"
#include "./src/config.h"

void run() {
    srand(time(NULL)); 
    InitWindow(WIDTH, HEIGHT, "Boid Simulation");
    SetTargetFPS(FPS);

    Boids* boids = InitBoids(NUM_BOIDS, SIZE_BOID);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        UpdateBoids(boids);
        DrawBoids(boids); 
        DrawFPS(5, 5);
        DrawText(TextFormat("%d BOIDS", NUM_BOIDS), 5, 25, 20, LIME); 
        EndDrawing();
    }

    CloseWindow();
    FreeBoids(boids);
}

int main(void) {
    run(); 
    return 0;
}
