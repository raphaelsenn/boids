#include "boid.h"

Boid* InitBoid(f32 x, f32 y, f32 radius) {
    Boid* boid = (Boid*)malloc(sizeof(Boid));
    if (!boid) return NULL;

    f32 angle = UNIFORM_AB(0, 2 * PI);
    f32 speed = UNIFORM_AB(V_MIN, V_MAX);
    boid->position = (Vector2){x, y};
    boid->velocity = (Vector2){cosf(angle) * speed, sinf(angle) * speed};
    boid->radius = radius;
    boid->neighbors = 0.0f;

    boid->closeToOthers = (Vector2){0.0f, 0.0f};
    boid->posAvg = (Vector2){0.0f, 0.0f};
    boid->velAvg = (Vector2){0.0f, 0.0f};
    boid->neighborAvg = (Vector2){0.0f, 0.0f};
    
    return boid;
}

void InitExistingBoid(Boid* boid, f32 x, f32 y, f32 radius) {
    if (!boid) return;

    f32 angle = UNIFORM_AB(0.0f, 2.0f * PI);
    f32 speed = UNIFORM_AB(V_MIN, V_MAX);
    boid->position = (Vector2){x, y};
    boid->velocity = (Vector2){cosf(angle) * speed, sinf(angle) * speed};
    boid->radius = radius;
    boid->neighbors = 0.0f;

    boid->closeToOthers = (Vector2){0.0f, 0.0f};
    boid->posAvg = (Vector2){0.0f, 0.0f};
    boid->velAvg = (Vector2){0.0f, 0.0f};
    boid->neighborAvg = (Vector2){0.0f, 0.0f};
}

void FreeBoid(Boid* boid) {
    if (!boid) return; 
    free(boid);
}

void DrawBoid(const Boid* boid) {
    if (!boid) return;
    DrawCircle(boid->position.x, boid->position.y, boid->radius, GetColorBoid(boid));
}

Color GetColorBoid(const Boid* boid) {
    if (!boid) return BLACK; 
    f32 t = (Vector2Length(boid->velocity) - V_MIN) / (V_MAX - V_MIN);
    Color out = {
        .r = (u8)(COLOR_SLOW.r + (COLOR_FAST.r - COLOR_SLOW.r) * t),
        .g = (u8)(COLOR_SLOW.g + (COLOR_FAST.g - COLOR_SLOW.g) * t),
        .b = (u8)(COLOR_SLOW.b + (COLOR_FAST.b - COLOR_SLOW.b) * t),
        .a = (u8)(COLOR_SLOW.a + (COLOR_FAST.a - COLOR_SLOW.a) * t),
    }; 
    return out; 
}

void UpdateBoid(Boid* boid) {
    if (!boid) return; 
    UpdateSeparationBoid(boid, AVOID_FACTOR);
    UpdateAlignmentBoid(boid, MATCHING_FACTOR);
    UpdateCohesionBoid(boid, CENTERING_FACTOR);
    HandleWallBoid(boid, TURN_FACTOR);
    HandleMinMaxSpeed(boid);
    UpdatePositionBoid(boid);    
}

void UpdatePositionBoid(Boid* boid) {
    boid->position = Vector2Add(boid->position, boid->velocity); 
}

void UpdateNeighborsBoid(Boid* boid) {
    boid->neighbors += 1.0f;
}

void UpdateSeparationBoid(Boid* boid, f32 avoid_factor) {
    if (boid->neighbors <= 0.0f) return;
    Vector2 dV = Vector2Scale(boid->closeToOthers, avoid_factor); 
    boid->velocity = Vector2Add(boid->velocity, dV);
}

void UpdateAlignmentBoid(Boid* boid, f32 matching_factor) {
    if (boid->neighbors <= 0.0f) return; 
    Vector2 dV = Vector2Subtract(boid->velAvg, boid->velocity);
    dV = Vector2Scale(dV, matching_factor);
    boid->velocity = Vector2Add(boid->velocity, dV);
}

void UpdateCohesionBoid(Boid* boid, f32 centering_factor) {
    if (boid->neighbors <= 0.0f) return; 
    Vector2 dP = Vector2Subtract(boid->posAvg, boid->position);
    dP = Vector2Scale(dP, centering_factor);
    boid->velocity = Vector2Add(boid->velocity, dP);
}

void ResetAvgsBoid(Boid* boid) {
    boid->neighbors = 0.0f; 
    boid->posAvg = (Vector2){0.0f, 0.0f};
    boid->velAvg = (Vector2){0.0f, 0.0f};
    boid->neighborAvg = (Vector2){0.0f, 0.0f};
    boid->closeToOthers = (Vector2){0.0f, 0.0f};
}

void HandleWallBoid(Boid* boid, f32 turn_factor) {
    f32 x = boid->position.x;    
    f32 y = boid->position.y;    

    if (x > WIDTH - LEFT_MARGIN) {
        boid->velocity.x -= turn_factor;
    } 
    if (x < RIGHT_MARGIN) {
        boid->velocity.x += turn_factor;
    } 
    if (y > HEIGHT - BOTTOM_MARGIN) {
        boid->velocity.y -= turn_factor;
    } 
    if (y < TOP_MARGIN) {
        boid->velocity.y += turn_factor;
    } 
}

void HandleMinMaxSpeed(Boid* boid) {
    f32 speed = Vector2Length(boid->velocity);

    if (speed > V_MAX) {
        boid->velocity.x = (boid->velocity.x / speed) * V_MAX;
        boid->velocity.y = (boid->velocity.y / speed) * V_MAX;
    } else if (speed < V_MIN) {
        boid->velocity.x = (boid->velocity.x / speed) * V_MIN;
        boid->velocity.y = (boid->velocity.y / speed) * V_MIN;
    }
}

void ComputeSeparationBoid(Boid* boid, Boid* other) {
    Vector2 dP = Vector2Subtract(boid->position, other->position);
    boid->closeToOthers = Vector2Add(boid->closeToOthers, dP);
}

void ComputeAlignmentBoid(Boid* boid, Boid* other) {
    // Compute velAvg using running mean
    Vector2 diff = Vector2Subtract(other->velocity, boid->velAvg);
    diff = Vector2Scale(diff, 1.0f / boid->neighbors);
    boid->velAvg = Vector2Add(boid->velAvg, diff); 
}

void ComputeCohesionBoid(Boid* boid, Boid* other) {
    // Compute posAvg using running mean
    Vector2 diff = Vector2Subtract(other->position, boid->posAvg);
    diff = Vector2Scale(diff, 1.0f / boid->neighbors);
    boid->posAvg = Vector2Add(boid->posAvg, diff); 
}


BoolStruct CheckLengths(Boid* boid, Boid* other) {
    f32 dx = boid->position.x - other->position.x;
    f32 dy = boid->position.y - other->position.y;
    f32 squarred = dx*dx + dy*dy; 
 
    BoolStruct bs = {
        fabs(dx) < VISUAL_RANGE && fabs(dy) < VISUAL_RANGE,
        squarred < VISUAL_RANGE * VISUAL_RANGE,
        squarred < PROTECTED_RANGE * PROTECTED_RANGE
    };
    return bs;    
}

void HandleBoidLogic(Boid* boid, Boid* other) {
    BoolStruct bs = CheckLengths(boid, other); 
    if (bs.is_visual_length) {
        if (bs.is_protected_length) { 
            ComputeSeparationBoid(boid, other);
        } else if (bs.is_visual_length_squarred) {
            UpdateNeighborsBoid(boid);
            ComputeAlignmentBoid(boid, other);
            ComputeCohesionBoid(boid, other);
        }
    }
}