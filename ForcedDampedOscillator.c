#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct _STATE {
    double x;
    double v;
} State;

typedef struct _DERIVATIVE {
    double dx;
    double dv;
} Derivative;

double k = 1.0;

double periodic_force(double t, double A, double omega, double phase) {
    return A*sin(omega*t + phase);
}

Derivative evaluate(State state, double k, double b, double m) {
    Derivative output;
    output.dx = state.v;
    output.dv = -k * state.x / m - b * state.v / m + periodic_force(0, 1, 1, 0) / m;
    return output;
}

void RK4(State *state, double dt, double k, double b, double m) {
    State initial = *state;
    
    Derivative k1 = evaluate(initial, k, b, m);
    
    State temp = {
        .x = initial.x + (k1.dx * dt/2),
        .v = initial.v + (k1.dv * dt/2)
    };
    Derivative k2 = evaluate(temp, k, b, m);
    
    temp.x = initial.x + (k2.dx * dt/2);
    temp.v = initial.v + (k2.dv * dt/2);
    Derivative k3 = evaluate(temp, k, b, m);
    
    temp.x = initial.x + (k3.dx * dt);
    temp.v = initial.v + (k3.dv * dt);
    Derivative k4 = evaluate(temp, k, b, m);
    
    state->x = initial.x + (dt/6.0) * 
        (k1.dx + 2*k2.dx + 2*k3.dx + k4.dx);
    state->v = initial.v + (dt/6.0) * 
        (k1.dv + 2*k2.dv + 2*k3.dv + k4.dv);
}

int main() {
    double dt = 0.01;
    double t_max = 50.0;
    double t = 0.0;
    double m = 1.0;
    double b = 0.5;
    double A = 1.0;
    double phase = 0.0;

    double frequencies[] = {0.5, 1.0, 2.0, 3.0};
    int num_frequencies = 4;
    
    for (int i = 0; i < num_frequencies; i++) {
        double omega = frequencies[i];
        t = 0.0;
        State state = {.x = 1.0, .v = 0.0}; 
        
        printf("Simulating with frequency: %.2f\n", omega);
        while (t < t_max) {
            printf("t=%.2f, x=%.6f, v=%.6f\n", t, state.x, state.v);
            RK4(&state, t, dt, k, b, m);
            t += dt;
        }
        printf("\n");
    }
    
    return 0;
}