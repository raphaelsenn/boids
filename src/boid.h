#ifndef BOID_H
#define BOID_H

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "raylib.h"
#include "raymath.h"

#include "types.h"
#include "config.h"

/* Samples of a uniform distribution over x ~ U[0, 1] */
#define UNIFORM() (f32)rand() / (f32)RAND_MAX

/* Samples of a uniform distribution over x ~ U[a, b], where a < b */
#define UNIFORM_AB(a, b) UNIFORM() * (f32)(b - a) + (f32)a

/* Helper struct. */
typedef struct {
    u8 is_visual_length;
    u8 is_visual_length_squarred;
    u8 is_protected_length;
} BoolStruct;

/* Simple struct describing a single boid. */
typedef struct {
    Vector2 position;
    Vector2 velocity;
    f32 radius;
    f32 neighbors;

    Vector2 closeToOthers;
    Vector2 posAvg, velAvg, neighborAvg;
} Boid;

Boid* InitBoid(f32 x, f32 y, f32 radius);
void InitExistingBoid(Boid* boid, f32 x, f32 y, f32 radius);
void FreeBoid(Boid* boid);

void DrawBoid(const Boid* boid);
Color GetColorBoid(const Boid* boid);

void UpdateBoid(Boid* boid);
void UpdatePositionBoid(Boid* boid);
void UpdateNeighborsBoid(Boid* boid);
void UpdateSeparationBoid(Boid* boid, f32 avoid_factor);
void UpdateAlignmentBoid(Boid* boid, f32 matching_factor);
void UpdateCohesionBoid(Boid* boid, f32 centering_factor);

void ResetAvgsBoid(Boid* boid);
void HandleWallBoid(Boid* boid, f32 turn_factor);
void HandleMinMaxSpeed(Boid* boid);
void HandleBoidLogic(Boid* boid, Boid* other);

void ComputeSeparationBoid(Boid* boid, Boid* other);
void ComputeAlignmentBoid(Boid* boid, Boid* other);
void ComputeCohesionBoid(Boid* boid, Boid* other);

BoolStruct CheckLengths(Boid* boid, Boid* other);

#endif // BOID_H