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

Derivative evaluate(State state, double length, double dt, Derivative k) {
    State temp = {
        .theta = state.theta + k.dtheta * dt,
        .omega = state.omega + k.domega * dt
    };
    Derivative output;
    output.dtheta = state.omega;
    output.domega = -(G / length) * sin(state.theta);
    return output;
}

void RK4(State *state, double dt, double length) {
    State initial = *state;
    
    Derivative k1 = evaluate(initial, length, dt, (Derivative){0, 0});
    Derivative k2 = evaluate(initial, length, dt/2, k1);
    Derivative k3 = evaluate(initial, length, dt/2, k2);
    Derivative k4 = evaluate(initial, length, dt, k3);
    
    state->theta = initial.theta + (dt/6.0) * 
        (k1.dtheta + 2*k2.dtheta + 2*k3.dtheta + k4.dtheta);
    state->omega = initial.omega + (dt/6.0) * 
        (k1.domega + 2*k2.domega + 2*k3.domega + k4.domega);
}

void simulate_pendulum(double init_theta, double init_omega,const char* filename, const char* store_folder, double length, double dt, double total_time) {
    State state = {init_theta, init_omega};
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
        // if (state.theta > 2*M_PI) {
        //     state.theta = -state.theta;
        // } else if (state.theta < -2*M_PI) {
        //     state.theta = -state.theta;
        // }
        RK4(&state, dt, length);
        t += dt;
    }
    fclose(data);
}

void Euler(State *state, double dt, double length) {
    State initial = *state;
    state->theta = initial.theta + initial.omega * dt;
    state->omega = initial.omega - (G / length) * sin(initial.theta) * dt;
}

void simulate_pendulum_euler(double init_theta, double init_omega,const char* filename, const char* store_folder, double length, double dt, double total_time) {
    State state = {init_theta, init_omega};
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
        if (state.theta > 2*M_PI) {
            state.theta = -state.theta;
        } else if (state.theta < -2*M_PI) {
            state.theta = -state.theta;
        }
        Euler(&state, dt, length);
        t += dt;
    }
    fclose(data);
}

int main() {
    double length = 1.0;
    double dt = 0.0001;
    double total_time = 5;
    int angle_deg;
    
    for (double theta = M_PI/6; theta <= 2*M_PI; theta += M_PI/4) {
        angle_deg = (int)(theta * 180.0 / M_PI);
        char filename[100];
        snprintf(filename, sizeof(filename), "pendulum_simulation_%d_deg.csv", angle_deg);
        simulate_pendulum(theta, 2, filename, "SimulationResultFinal", length, dt, total_time);
        snprintf(filename, sizeof(filename), "pendulum_simulation_%d_m_deg.csv", angle_deg);
        simulate_pendulum(theta, -2, filename, "SimulationResultFinal", length, dt, total_time);
    }

// for (double theta = M_PI/6; theta <= 2*M_PI; theta += M_PI/4) {
//     angle_deg = (int)(theta * 180.0 / M_PI);
//     char filename[100];
//     snprintf(filename, sizeof(filename), "pendulum_simulation_%d_deg.csv", angle_deg);
//     simulate_pendulum_euler(theta, 0, filename, "EulerSimulationResultO0", length, dt, total_time);
// }
    return 0;
}