#include "unity.h"
#include "raylib.h"
#include "boid.h"

#define DELTA 1e-6

void setUp(void) {}
void tearDown(void) {}

void test_init_boid(void) {
    Boid* boid = InitBoid(1.0f, -1.0f, 1.0f);

    TEST_ASSERT_FLOAT_WITHIN(DELTA, 1.0f, boid->position.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, -1.0f, boid->position.y);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 1.0f, boid->radius);

    FreeBoid(boid);
}

void test_update_position_boid(void) {
    Boid* boid = InitBoid(0.0f, 0.0f, 1.0f);
    
    boid->velocity.x = 0.5;
    boid->velocity.y = -0.5;
    UpdatePositionBoid(boid);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.5f, boid->position.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, -0.5f, boid->position.y);
    
    boid->velocity.x = -2.5;
    boid->velocity.y = 2.5;
    UpdatePositionBoid(boid);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, -2.0f, boid->position.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.0f, boid->position.y);
    
    FreeBoid(boid);
}

void test_update_neighbors_boid(void) {
    Boid* boid = InitBoid(0.0f, 0.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->neighbors);

    UpdateNeighborsBoid(boid);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 1.0f, boid->neighbors);
    
    UpdateNeighborsBoid(boid);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.0f, boid->neighbors);
    
    UpdateNeighborsBoid(boid);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 3.0f, boid->neighbors);

    FreeBoid(boid);
}

void test_reset_avgs_boid(void) {
    Boid* boid = InitBoid(0.0f, 0.0f, 1.0f);
    boid->posAvg = (Vector2){UNIFORM(), UNIFORM()};
    boid->velAvg = (Vector2){UNIFORM(), UNIFORM()};
    boid->neighborAvg = (Vector2){UNIFORM(), UNIFORM()};
    boid->closeToOthers = (Vector2){UNIFORM(), UNIFORM()};

    ResetAvgsBoid(boid);

    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->posAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->posAvg.y);
    
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->velAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->velAvg.y);
    
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->neighborAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->neighborAvg.y);
   
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->closeToOthers.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.0f, boid->closeToOthers.y);

    FreeBoid(boid);
}

void test_compute_separation_boid(void) {
    Boid* boid = InitBoid(0.0f, 0.0f, 1.0f);
    Boid* other = InitBoid(1.0f, 2.0f, 1.0f);

    ComputeSeparationBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, -1.0f, boid->closeToOthers.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, -2.0f, boid->closeToOthers.y);

    other->position.x = -1.5;
    other->position.y = -2.5;
    
    ComputeSeparationBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.5f, boid->closeToOthers.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 0.5f, boid->closeToOthers.y);

    FreeBoid(boid);
    FreeBoid(other);
}

void test_compute_alignment_boid(void) {
    Boid* boid = InitBoid(0.0f, 0.0f, 1.0f);
    Boid* other = InitBoid(0.0f, 0.0f, 1.0f);
    
    // Running mean 
    other->velocity.x = 1.0f; 
    other->velocity.y = 2.0f; 
    UpdateNeighborsBoid(boid); 
    ComputeAlignmentBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 1.0f, boid->velAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.0f, boid->velAvg.y);
    
    other->velocity.x = 2.0f;
    other->velocity.y = 3.0f;
    UpdateNeighborsBoid(boid);
    ComputeAlignmentBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 1.5f, boid->velAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.5f, boid->velAvg.y);
    
    other->velocity.x = 3.0f;
    other->velocity.y = 4.0f;
    UpdateNeighborsBoid(boid);
    ComputeAlignmentBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.0f, boid->velAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 3.0f, boid->velAvg.y);
    
    other->velocity.x = 4.0f;
    other->velocity.y = 5.0f;
    UpdateNeighborsBoid(boid);
    ComputeAlignmentBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.5f, boid->velAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 3.5f, boid->velAvg.y);
    
    other->velocity.x = 5.0f;
    other->velocity.y = 6.0f;
    UpdateNeighborsBoid(boid);
    ComputeAlignmentBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 3.0f, boid->velAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 4.0f, boid->velAvg.y);

    FreeBoid(boid);
    FreeBoid(other);
}

void test_compute_cohesion_boid(void) {
    Boid* boid = InitBoid(0.0f, 0.0f, 1.0f);
    Boid* other = InitBoid(1.0f, 2.0f, 1.0f);
    
    // Running mean 
    UpdateNeighborsBoid(boid); 
    ComputeCohesionBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 1.0f, boid->posAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.0f, boid->posAvg.y);
    
    other->position.x = 2.0f;
    other->position.y = 3.0f;
    UpdateNeighborsBoid(boid);
    ComputeCohesionBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 1.5f, boid->posAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.5f, boid->posAvg.y);
    
    other->position.x = 3.0f;
    other->position.y = 4.0f;
    UpdateNeighborsBoid(boid);
    ComputeCohesionBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.0f, boid->posAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 3.0f, boid->posAvg.y);
    
    other->position.x = 4.0f;
    other->position.y = 5.0f;
    UpdateNeighborsBoid(boid);
    ComputeCohesionBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 2.5f, boid->posAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 3.5f, boid->posAvg.y);
    
    other->position.x = 5.0f;
    other->position.y = 6.0f;
    UpdateNeighborsBoid(boid);
    ComputeCohesionBoid(boid, other);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 3.0f, boid->posAvg.x);
    TEST_ASSERT_FLOAT_WITHIN(DELTA, 4.0f, boid->posAvg.y);

    FreeBoid(boid);
    FreeBoid(other);
}

/* Compile: clang -I"/Users/raphael/c/Unity/src/" test_bandit.c /Users/raphael/c/Unity/src/unity.c */
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_boid); 
    RUN_TEST(test_update_position_boid); 
    RUN_TEST(test_update_neighbors_boid); 
    RUN_TEST(test_reset_avgs_boid); 
    RUN_TEST(test_compute_separation_boid); 
    RUN_TEST(test_compute_alignment_boid); 
    RUN_TEST(test_compute_cohesion_boid); 
    return UNITY_END();
}
