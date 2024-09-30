#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"


//numbers in this 

double k = 9.8; // elastic constant (assume the rope is a spring with extremely big k)
double m = 1; //mass (kg)

vec v;
vec a;
vec extend;
vec gravity  = {0, -9.8};

void move(vec *pos, vec *v, vec *a, double dt) {
    pos->x = pos->x + v->x*dt;
    pos->y = pos->y + v->y*dt;
    v->x += a->x*dt;
    v->y += a->y*dt;
}

vec spring_force(double k, vec extend, vec direction) {
    vec force;
    force.x = -extend.x * k;
    force.y = -extend.y * k;
    return force;
}

void accelerate(vec *acceleration, vec force, double mass, double dt) {
    acceleration->x = (force.x / m) * dt;
    acceleration->y = (force.y / m) * dt;
}

vec step(vec state, double t, double dt) {
    move(&state, &v, &a, t);
    vec next_state;
    vec f = spring_force(k, extend, times_vec(&extend, norm(&extend)));
    vec resultant = add_vec(&f, &gravity);

    state.x = state.x + v.x * dt;
    state.y = state.y + v.y * dt;

    accelerate(&a, resultant, m, dt);

    return state;
}

double step_state(vec *state, double t) {
    vec k1, k2, k3, k4;
    vec temp;
    double h = 0.001;
    k1 = step(*state, t, 0);
    temp = times_vec(&k1, h/2);
    k2 = step(add_vec(state, &temp), t, h/2);
    temp = times_vec(&k3, h/2);
    k3 = step(add_vec(state, &temp), t, h/2);
    temp = times_vec(&k3, h);
    k4 = step(add_vec(state, &temp), t, h);
    temp = add_vec(&k1, &k4);
    k2 = times_vec(&k2, 2);
    k3 = times_vec(&k3, 2);
    temp = add_vec(&temp, &k2);
    temp = add_vec(&temp, &k3);
    temp =  times_vec(&temp, h/6);
    *state = add_vec(state, &temp);
    t += h;
    return t;
}


int main() {
    FILE *data;
    double time = 0;
    double dt = 0.001;
    double force_mag;
    vec item_pos;
    item_pos = polar_coord(10, 3.14*(4/3));
    vec rope_start;
    rope_start.x = 0.0;
    rope_start.y = 0.0;
    data = fopen("SimulationResult/simulation_data.csv", "w+");
    fprintf(data, "TIME, POSITION X, POSITION Y, VELOCITY, ACCELERATION\n"); 
    while (time < 100) {
        extend = minus_vec(&item_pos, &rope_start);
        time = step_state(&item_pos, time);
        fprintf(data, "%.3f, %.3f, %.3f, %.3f, %.3f\n", time, item_pos.x, item_pos.y, norm(&v), norm(&a));
    }   
    fclose(data);
    return 0;
}