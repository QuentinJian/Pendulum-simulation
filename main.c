#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 9.81 


typedef struct {
    double theta; 
    double omega; 
} State;


typedef struct {
    double dtheta; 
    double domega; 
} Derivative;


Derivative evaluate(State *state, double length) {
    Derivative output;
    output.dtheta = state->omega;
    output.domega = -(G / length) * sin(state->theta);
    return output;
}

void RK4(State *state, double dt, double length) {
    Derivative k1, k2, k3, k4;

    k1 = evaluate(state, length);
    State state1 = {state->theta + 0.5 * dt * k1.dtheta, state->omega + 0.5 * dt * k1.domega};
    k2 = evaluate(&state1, length);
    State state2 = {state->theta + 0.5 * dt * k2.dtheta, state->omega + 0.5 * dt * k2.domega};
    k3 = evaluate(&state2, length);
    State state3 = {state->theta + dt * k3.dtheta, state->omega + dt * k3.domega};
    k4 = evaluate(&state3, length);
    
    state->theta += (dt / 6.0) * (k1.dtheta + 2.0 * k2.dtheta + 2.0 * k3.dtheta + k4.dtheta);
    state->omega += (dt / 6.0) * (k1.domega + 2.0 * k2.domega + 2.0 * k3.domega + k4.domega);
}

int main() {
    State state = {3.14159/4, 0}; 
    double length = 1.0; 
    double t = 0.0;
    double dt = 0.01; 
    
    FILE *data = fopen("pendulum_simulation.csv", "w+");
    fprintf(data, "TIME,ANGLE,ANGULAR_VELOCITY\n");
    while (t < 10.0) {
        fprintf(data, "%.2f, %.6f, %.6f\n", t, state.theta, state.omega);
        RK4(&state, dt, length);
        t += dt;
    }
    fclose(data);
    t = 0;
    state.theta = 3.14*(2.0/3.0);
    state.omega = 0; 
    data = fopen("pendulum_simulation_pi6.csv", "w+");
    fprintf(data, "TIME,ANGLE,ANGULAR_VELOCITY\n");
    while (t < 10.0) {
        fprintf(data, "%.2f, %.6f, %.6f\n", t, state.theta, state.omega);
        RK4(&state, dt, length);
        t += dt;
    }
    fclose(data);
    t = 0;
    state.theta = 3.14159;
    state.omega = 0;
    data = fopen("pendulum_simulation_pi.csv", "w+");
    fprintf(data, "TIME,ANGLE,ANGULAR_VELOCITY\n");
    while (t < 10.0) {
        fprintf(data, "%.2f, %.6f, %.6f\n", t, state.theta, state.omega);
        RK4(&state, dt, length);
        t += dt;
    }
    fclose(data);
    state.theta = -3.14159;
    state.omega = 0;
    data = fopen("pendulum_simulation_pi_m.csv", "w+");
    t = 0;
    fprintf(data, "TIME,ANGLE,ANGULAR_VELOCITY\n");
    while (t < 10.0) {
        fprintf(data, "%.2f, %.6f, %.6f\n", t, state.theta, state.omega);
        RK4(&state, dt, length);
        t += dt;
    }
    fclose(data);
    return 0;
}
