#ifndef CONFIG_H
#define CONFIG_H

/* Raylib window settings */

#define WIDTH 1280 
#define HEIGHT 720 
#define FPS 30

/* Boid simulation settings */

#define NUM_BOIDS 8000          // Number of boids in simulation
#define SIZE_BOID 3.0f          // Size of a boid (radius) in pixels

#define COLOR_FAST RED          // Boids with maximum velocity are red
#define COLOR_SLOW BLUE         // Boids with minimum velocity are blue

#define V_MAX 6.0f              // Maximum velocity of a boid
#define V_MIN 3.0f              // Minimum velocity of a boid

#define SPWAN_OFFSET 100.0f     // Spawn offset around center

#define VISUAL_RANGE 40.0f      // Neighbor detection radius (alignment and cohesion)
#define PROTECTED_RANGE 8.0f    // Short range radius (avoid collision)

/* Boid behavior factors */

#define AVOID_FACTOR 0.05f      // Strength of separation
#define MATCHING_FACTOR 0.05f   // Strength of alignment    
#define CENTERING_FACTOR 0.0005f// Strength of cohesion
#define TURN_FACTOR 0.2f        // Strength of turning

/* Screen boundary margins for boids */

#define LEFT_MARGIN 80.0f
#define RIGHT_MARGIN 80.0f
#define TOP_MARGIN 80.0f
#define BOTTOM_MARGIN 80.0f

#endif // CONFIG_H