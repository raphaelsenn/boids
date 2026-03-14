#include "boid_manager.h"
#include <stdio.h>

/* The spatial hashgrid implementation is inspired from the java script implementation from Ten Minute Physics
 * Reference:
 * Github: https://github.com/matthias-research/pages/blob/master/tenMinutePhysics/11-hashing.html
 * Pdf about the data structure: https://matthias-research.github.io/pages/tenMinutePhysics/11-hashing.pdf
 * Video about the data structure: https://www.youtube.com/watch?v=D2M8jTtKi44
 */

HashGrid* InitGrid(f32 spacing, u32 max_num_objects) {
    HashGrid* grid = (HashGrid*)malloc(sizeof(HashGrid));
    grid->spacing = spacing;
    
    u32 table_size = 2 * max_num_objects;
    grid->table_size = table_size;
    grid->max_num_objects = max_num_objects;

    grid->cell_start = (u32*)malloc((table_size + 1) * sizeof(u32));
    grid->cell_entries = (u32*)malloc(max_num_objects * sizeof(u32));
    grid->query_ids = (u32*)malloc(max_num_objects * sizeof(u32));
    grid->query_size = 0;

    return grid;
}

void FreeGrid(HashGrid* grid) {
    free(grid->cell_start);
    free(grid->cell_entries);
    free(grid->query_ids);
    FreeGrid(grid);
}

u32 HashCords(HashGrid* grid, i32 x, i32 y) {
    u32 h = (u32)(x * 92837111u) ^ (u32)(y * 689287499u);
    return h % grid->table_size;
}

i32 IntCord(HashGrid* grid, f32 coord) {
    return floorf(coord / grid->spacing);
}

Vector2 HashPos(HashGrid* grid, Boid* boid) {
    f32 x = boid->position.x;
    f32 y = boid->position.y;
    i32 i = IntCord(grid, x);
    i32 j = IntCord(grid, y);
    return (Vector2){i, j};
}

/* Inspired from: 
 *https://github.com/matthias-research/pages/blob/master/tenMinutePhysics/11-hashing.html 
 */
void ReCreateGrid(HashGrid* grid, Boids* boids) {
    if (!grid || !boids) return;

    for (u32 i = 0; i < grid->table_size + 1; i++) {
        grid->cell_start[i] = 0;
    }

    for (u32 i = 0; i < boids->num_boids; i++) {
        i32 cx = IntCord(grid, boids->boids[i].position.x);
        i32 cy = IntCord(grid, boids->boids[i].position.y);
        u32 h = HashCords(grid, cx, cy);
        grid->cell_start[h]++;
    }

    u32 start = 0;
    for (u32 i = 0; i < grid->table_size; i++) {
        u32 count = grid->cell_start[i];
        grid->cell_start[i] = start;
        start += count;
    }
    grid->cell_start[grid->table_size] = start;

    for (u32 i = 0; i < boids->num_boids; i++) {
        i32 cx = IntCord(grid, boids->boids[i].position.x);
        i32 cy = IntCord(grid, boids->boids[i].position.y);
        u32 h = HashCords(grid, cx, cy);

        grid->cell_entries[grid->cell_start[h]] = i;
        grid->cell_start[h]++;
    }

    for (u32 i = grid->table_size; i > 0; i--) {
        grid->cell_start[i] = grid->cell_start[i - 1];
    }
    grid->cell_start[0] = 0;
}

/* Inspired from: 
 *https://github.com/matthias-research/pages/blob/master/tenMinutePhysics/11-hashing.html 
 */
void QueryGrid(HashGrid* grid, Boids* boids, u32 nr, f32 max_dist) {
    if (!grid || !boids) return;
    if (nr >= boids->num_boids) return;

    f32 x = boids->boids[nr].position.x;
    f32 y = boids->boids[nr].position.y;

    i32 x0 = IntCord(grid, x - max_dist);
    i32 y0 = IntCord(grid, y - max_dist);
    i32 x1 = IntCord(grid, x + max_dist);
    i32 y1 = IntCord(grid, y + max_dist);

    grid->query_size = 0;

    for (i32 xi = x0; xi <= x1; xi++) {
        for (i32 yi = y0; yi <= y1; yi++) {
            u32 h = HashCords(grid, xi, yi);
            u32 start = grid->cell_start[h];
            u32 end = grid->cell_start[h + 1];

            for (u32 i = start; i < end; i++) {
                if (grid->query_size >= grid->max_num_objects) {
                    return;
                }
                grid->query_ids[grid->query_size++] = grid->cell_entries[i];
            }
        }
    }
}

Boids* InitBoids(u32 num_boids, f32 radius) {
    Boids* boids = (Boids*)malloc(sizeof(Boids));
    boids->boids = (Boid*)malloc(num_boids * sizeof(Boid));
    boids->num_boids = num_boids;

    for (u32 b = 0; b < boids->num_boids; b++) {
        f32 x = (f32)((WIDTH / 2)) + UNIFORM_AB(-SPWAN_OFFSET, SPWAN_OFFSET);
        f32 y = (f32)((HEIGHT / 2)) + UNIFORM_AB(-SPWAN_OFFSET, SPWAN_OFFSET);
        InitExistingBoid(&boids->boids[b], x, y, radius);
    }
    
    boids->grid = InitGrid(VISUAL_RANGE, NUM_BOIDS);
    return boids;
}

void FreeBoids(Boids* boids) {
    if (!boids) return; 
    free(boids->boids); 
    free(boids);
}

void DrawBoids(const Boids* boids) {
    for (u32 b = 0; b < boids->num_boids; b++) {
        DrawBoid(&boids->boids[b]);
    }
}

void UpdateBoids(Boids* boids) {
    if (!boids || !boids->grid) return;

    // Reset neighbor stats
    for (u32 i = 0; i < boids->num_boids; i++) {
        ResetAvgsBoid(&boids->boids[i]);
    }

    // Rebuild hash-grid 
    ReCreateGrid(boids->grid, boids);

    // Search neighbors, should run in O(n * k),
    // where k is the average neighbor number 
    for (u32 i = 0; i < boids->num_boids; i++) {
        Boid* boid = &boids->boids[i];

        QueryGrid(boids->grid, boids, i, VISUAL_RANGE);
        
        // Runs in O(k), where k is the neighbor size
        for (u32 k = 0; k < boids->grid->query_size; k++) {
            u32 j = boids->grid->query_ids[k];

            if (j == i) { continue; }

            Boid* other = &boids->boids[j];

            f32 dx = other->position.x - boid->position.x;
            f32 dy = other->position.y - boid->position.y;
            f32 dist2 = dx * dx + dy * dy;

            if (dist2 <= VISUAL_RANGE * VISUAL_RANGE) {
                HandleBoidLogic(boid, other);
            }
        }
    }

    // Upate boid
    for (u32 i = 0; i < boids->num_boids; i++) {
        UpdateBoid(&boids->boids[i]);
    }
}