#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 9.81 
#define M_PI 3.14159265358979323846

typedef struct {
    double theta; 
    double omega; 
} State;

typedef struct {
    double dtheta; 
    double domega; 
} Derivative;

Derivative evaluate(State state, double length) {
    Derivative output;
    output.dtheta = state.omega;
    output.domega = -(G / length) * sin(state.theta);
    return output;
}

void RK4(State *state, double dt, double length) {
    State initial = *state;
    
    Derivative k1 = evaluate(initial, length);
    
    State temp = {
        .theta = initial.theta + (k1.dtheta * dt/2),
        .omega = initial.omega + (k1.domega * dt/2)
    };
    Derivative k2 = evaluate(temp, length);
    
    temp.theta = initial.theta + (k2.dtheta * dt/2);
    temp.omega = initial.omega + (k2.domega * dt/2);
    Derivative k3 = evaluate(temp, length);
    
    temp.theta = initial.theta + (k3.dtheta * dt);
    temp.omega = initial.omega + (k3.domega * dt);
    Derivative k4 = evaluate(temp, length);
    
    state->theta = initial.theta + (dt/6.0) * 
        (k1.dtheta + 2*k2.dtheta + 2*k3.dtheta + k4.dtheta);
    state->omega = initial.omega + (dt/6.0) * 
        (k1.domega + 2*k2.domega + 2*k3.domega + k4.domega);
}

void simulate_pendulum(double init_theta, double init_omega,const char* filename, const char* store_folder, double length, double dt, double total_time) {
    State state = {init_theta, 0};
    double t = 0.0;
    char path[100];
    sprintf(path, "%s/%s", store_folder, filename);
    
    FILE *data = fopen(path, "w+");
    if (!data) {
        fprintf(stderr, "Error opening file %s\n", path);
        return;
    }
    
    fprintf(data, "TIME,ANGLE,ANGULAR_VELOCITY\n");
    while (t < total_time) {
        fprintf(data, "%.2f,%.6f,%.6f\n", t, state.theta, state.omega);
        RK4(&state, dt, length);
        t += dt;
    }
    fclose(data);
}

int main() {
    double length = 1.0;
    double dt = 0.01;
    double total_time = 10.0;
    int angle_deg;
    
    for (double theta = M_PI/8; theta <= 2*M_PI; theta += M_PI/8) {
        angle_deg = (int)(theta * 180.0 / M_PI);
        char filename[100];
        snprintf(filename, sizeof(filename), "pendulum_simulation_%d_deg.csv", angle_deg);
        simulate_pendulum(theta, 0, filename, "SimulationResultTest", length, dt, total_time);
    }
    return 0;
}