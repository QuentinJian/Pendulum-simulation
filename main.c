#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "vector.h"


//numbers in this 

double k = 100000; // elastic constant (assume the rope is a spring with extremely big k)
double m = 1; //mass (kg)



typedef struct vec { //define a new structure called vec (vector)
    double x;
    double y;
};

struct vec force;
struct vec v;
struct vec a;

void move(struct vec *pos, struct vec *v, struct vec *a, double dt) {
    pos->x = pos->x + v->x*dt;
    pos->y = pos->y + v->y*dt;
    v->x += a->x*dt;
    v->y += a->y*dt;
}

void accelerate(struct vec *acceleration, double dt) {

}

struct vec step(struct vec *state, double t, struct vec *velocity, struct vec *accleration) {
    move(state, velocity, accleration, t);
    
}

void step_state(struct vec *location, double t) {
    struct vec k1, k2, k3, k4;
    double h = 0.001;
    k1 = step(&location, t, &v, &a);
    // k2 = step()
}

double length(struct vec a) {
    double square = pow(a.x, 2) + pow(a.y, 2);
    return sqrt(square);
}

int main() {
    double time = 0;
    double force_mag;
    struct vec item_pos;
    item_pos.x = 0.0;
    item_pos.y = -10.0;
    struct vec rope_start;
    rope_start.x = 0.0;
    rope_start.y = 0.0;
    struct vec force;
    while (time < 1000) {
        struct vec extend_pos;
        extend_pos = minus_vec(&item_pos, &rope_start);
    }

}