#include <stdio.h>


typedef struct {
    double x;  
    double v;  
} State;


typedef struct {
    double dx; 
    double dv; 
} Derivative;


Derivative evaluate(const State *state, double t) {
    Derivative output;
    double k = 1.0; 
    double m = 1.0; 
    output.dx = state->v;
    output.dv = -(k / m) * state->x;
    return output;
}


void RK4(State *state, double t, double dt) {
    Derivative k1 = evaluate(state, t);
    State state1 = {state->x + 0.5 * dt * k1.dx, state->v + 0.5 * dt * k1.dv};
    Derivative k2 = evaluate(&state1, t + 0.5 * dt);
    State state2 = {state->x + 0.5 * dt * k2.dx, state->v + 0.5 * dt * k2.dv};
    Derivative k3 = evaluate(&state2, t + 0.5 * dt);
    State state3 = {state->x + dt * k3.dx, state->v + dt * k3.dv};
    Derivative k4 = evaluate(&state3, t + dt);

    state->x += (dt / 6.0) * (k1.dx + 2.0 * k2.dx + 2.0 * k3.dx + k4.dx);
    state->v += (dt / 6.0) * (k1.dv + 2.0 * k2.dv + 2.0 * k3.dv + k4.dv);
}

int main() {
    State state = {1.0, 0.0}; 
    double t = 0.0;
    double dt = 0.01; 

    FILE *data = fopen("simulation_data.csv", "w+");
    fprintf(data, "TIME,POSITION,VELOCITY\n");

    while (t < 10.0) {
        fprintf(data, "%.2f, %.6f, %.6f\n", t, state.x, state.v);
        RK4(&state, t, dt);
        t += dt;
    }

    fclose(data);
    return 0;
}
