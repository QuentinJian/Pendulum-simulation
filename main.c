#include <stdlib.h>
#include <iostream>
#include <math.h>


//numbers in this 

double k = 100000; // elastic constant (assume the rope is a spring with extremely big k)
double m = 1; //mass (kg)


struct vec { //define a new structure called vec (vector)
    double x;
    double y;
} position, velocity, accleration;

struct vec force;

void move(struct vec *pos, struct vec *v, struct vec *a, double dt) {
    pos->x = pos->x + v->x*dt;
    pos->y = pos->y + v->y*dt;
    v->x += a->x*dt;
    v->y += a->y*dt;

}



void step_state(struct vec *location, double dt) {
    double k;
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
        extend_pos.x = 
        force_mag = length()
    }

}