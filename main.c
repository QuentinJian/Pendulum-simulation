#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vector.h"

double k = 100000; 
double m = 1;     
double x0 = 10;   
vec extend;
vec gravity = {0, -9.8};  

typedef struct State {
    vec pos;
    vec v;
    vec a;
} State;

typedef struct Derivative {
    vec dr;
    vec dv;
} Derivative;

vec force_func(double k, double x0, vec extend) {
    vec unit_extend = norm(extend);
    return scale_vec(unit_extend, -k * (mag(extend) - x0)); 
}

Derivative evaluate(State *initial, double t, double dt, Derivative derivative) {
    State state;
    state.pos = add_vec(initial->pos, scale_vec(derivative.dr, dt));
    state.v = add_vec(initial->v, scale_vec(derivative.dv, dt));
    
    Derivative output;
    output.dr = state.v;
    
    vec F = add_vec(force_func(k, x0, extend), gravity);
    vec a = scale_vec(F, 1.0 / m); // acceleration = F / m
    
    output.dv = a;
    return output;
}

double RK4(State *cur_state, double t, double h) {
    Derivative k1, k2, k3, k4;
    Derivative cur_derivative = {cur_state->v, cur_state->a};
    
    k1 = evaluate(cur_state, t, 0, cur_derivative);
    k2 = evaluate(cur_state, t, h/2, k1);
    k3 = evaluate(cur_state, t, h/2, k2);
    k4 = evaluate(cur_state, t, h, k3);
    
    vec total_dr = scale_vec(add_vec(add_vec(scale_vec(k1.dr, 1), scale_vec(k2.dr, 2)), add_vec(scale_vec(k3.dr, 2), scale_vec(k4.dr, 1))), 1.0 / 6);
    vec total_dv = scale_vec(add_vec(add_vec(scale_vec(k1.dv, 1), scale_vec(k2.dv, 2)), add_vec(scale_vec(k3.dv, 2), scale_vec(k4.dv, 1))), 1.0 / 6);
    
    cur_state->v = add_vec(cur_state->v, scale_vec(total_dv, h));
    cur_state->pos = add_vec(cur_state->pos, scale_vec(total_dr, h));
    
    return t + h;
}

int main() {
    FILE *data;
    double time = 0;
    double dt = 0.01; // Decrease time step for better accuracy
    vec item_pos = {0, 10};  
    vec rope_start = {0.0, 0.0}; 
    
    extend = minus_vec(&item_pos, &rope_start); 
    x0 = mag(extend);  
    
    data = fopen("simulation_data.csv", "w+"); 
    fprintf(data, "TIME,POSITION_X,POSITION_Y,VELOCITY_X,VELOCITY_Y,ACCELERATION_X,ACCELERATION_Y\n");
    
    State cur_state = {item_pos, {0, 0}, {0, 0}};  
    extend = minus_vec(&cur_state.pos, &rope_start);  
    
    while (time < 10) {  
        extend = minus_vec(&cur_state.pos, &rope_start);
        
        vec F = add_vec(force_func(k, x0, extend), gravity);
        cur_state.a = scale_vec(F, 1.0 / m);
        
        time = RK4(&cur_state, time, dt);
        fprintf(data, "%.3f, %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", time, cur_state.pos.x, cur_state.pos.y, cur_state.v.x, cur_state.v.y, cur_state.a.x, cur_state.a.y);
    }
    
    fclose(data);
    return 0;
}
