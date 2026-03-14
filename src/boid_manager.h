#ifndef BOID_MANAGER_H
#define BOID_MANAGER_H

#include <stdlib.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "boid.h"
#include "types.h"
#include "config.h"

#if !defined(CONFIG_H)
#error "Please define  CONFIG_H."
#endif

/* The spatial hashgrid implementation is inspired from the java script implementation from Ten Minute Physics
 * Reference:
 * Github: https://github.com/matthias-research/pages/blob/master/tenMinutePhysics/11-hashing.html
 * Pdf about the data structure: https://matthias-research.github.io/pages/tenMinutePhysics/11-hashing.pdf
 * Video about the data structure: https://www.youtube.com/watch?v=D2M8jTtKi44
 */

typedef struct {
    f32 spacing;
    u32 table_size;
    u32 max_num_objects;

    u32* cell_start;
    u32* cell_entries;
    
    u32* query_ids;
    u32 query_size;
} HashGrid;

/* Boid manager. */ 
typedef struct {
    u32 num_boids;  
    Boid* boids;
    HashGrid* grid;    
} Boids;

Boids* InitBoids(u32 num_boids, f32 radius);
void FreeBoids(Boids* boids);
void DrawBoids(const Boids* boids);
void UpdateBoids(Boids* boids);

HashGrid* InitGrid(f32 spacing, u32 max_num_objects);
void FreeGrid(HashGrid* grid);
u32 HashCords(HashGrid* grid, i32 x, i32 y);
i32 IntCord(HashGrid* grid, f32 coord);
Vector2 HashPos(HashGrid* grid, Boid* boid);
void ReCreateGrid(HashGrid* grid, Boids* boids);
void QueryGrid(HashGrid* grid, Boids* boids, u32 nr, f32 max_dist);

#endif // BOID_MANAGER_H