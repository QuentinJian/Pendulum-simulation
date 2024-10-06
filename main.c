#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"


double k = 100000; 
double m = 1;     
double x0 = 10;   


vec extend;
vec gravity = {0, -9.8};  

typedef struct state {
    vec pos;   
    vec v;     
    vec a;     
} state;


vec force_func(double k, double x0, vec extend) {
    vec unit_extend = norm(extend);
    return scale_vec(unit_extend, -k * (mag(extend) - x0)); 
}

state evaluate(state *initial, double t, double dt, state prev) {
    state next_state;
    next_state.pos.x = initial->pos.x + prev.v.x*dt;
    next_state.pos.y = initial->pos.y + prev.v.y*dt;
    vec resultant = add_vec(force_func(k, x0, extend), gravity);
    next_state.a = scale_vec(resultant, 1/m);
    next_state.v = add_vec(next_state.v, scale_vec(next_state.a, dt));
    return next_state;
}

double RK4(state *cur_state, double t, double h) {
    state k1, k2, k3, k4;
    k1 = evaluate(cur_state, t, h, *cur_state);
    k2 = evaluate(cur_state, t, h*0.5, k1);
    k3 = evaluate(cur_state, t, h*0.5, k2);
    k4 = evaluate(cur_state, t, h, k3);
    vec temp = add_vec(k1.v, k4.v);
    temp = add_vec(temp, scale_vec(k2.v, 2));
    temp = add_vec(temp, scale_vec(k3.v, 2));
    vec v =  scale_vec(temp, 1/6);
    temp = add_vec(k1.a, k4.a);
    temp = add_vec(temp, scale_vec(k2.a, 2));
    temp = add_vec(temp, scale_vec(k3.a, 2));
    vec a = scale_vec(temp, 1/6);
    cur_state->pos = add_vec(cur_state->pos, scale_vec(v, h));
    cur_state->v = add_vec(cur_state->v, scale_vec(a, h));
    cur_state->a = a;
    return t + h;
}

int main() {
    FILE *data;
    double time = 0;
    double dt = 0.001;
    vec item_pos = {10, 0};  
    vec rope_start = {0.0, 0.0};       

    extend = minus_vec(&item_pos, &rope_start); 
    x0 = mag(extend);  

    data = fopen("simulation_data.csv", "w+"); 
    fprintf(data, "TIME,POSITION_X,POSITION_Y,VELOCITY,ACCELERATION\n");
    state cur_state = {item_pos, {0, 0}, {0, 0}};
    while (time < 10) {  
        extend = minus_vec(&cur_state.pos, &rope_start);  
        time = RK4(&cur_state, time, dt);                 
        fprintf(data, "%.3f, %.3f, %.3f, %.3f, %.3f\n", time, cur_state.pos.x, cur_state.pos.y, mag(cur_state.v), mag(cur_state.a));
    }

    fclose(data);
    return 0;
}

