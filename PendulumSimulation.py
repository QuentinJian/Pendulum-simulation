from vpython import *
import math

g = 9.81
L = 1.0
dt = 0.01

scene = canvas(title="Pendulum Simulation", width=800, height=600)

pivot = vector(0, 0, 0)
pivot_sphere = sphere(pos=pivot, radius=0.05, color=color.green)


class PendulumState:
    def __init__(self, theta=0.0, omega=0.0):
        self.theta = theta
        self.omega = omega


class Derivative:
    def __init__(self, dtheta=0.0, domega=0.0):
        self.dtheta = dtheta
        self.domega = domega


def evaluate(state, dt, k: Derivative):
    temp_theta = state.theta + k.dtheta * dt
    temp_omega = state.omega + k.domega * dt
    
    out = Derivative()
    out.dtheta = temp_omega
    out.domega = -(g/L)*math.sin(temp_theta)
    return out

def RK4(state: PendulumState, h):
    k1 = evaluate(state, 0, Derivative())
    k2 = evaluate(state, h/2, k1)
    k3 = evaluate(state, h/2, k2)
    k4 = evaluate(state, h, k3)
    
    state.theta += (h/6.0) * (k1.dtheta + 2*k2.dtheta + 2*k3.dtheta + k4.dtheta)
    state.omega += (h/6.0) * (k1.domega + 2*k2.domega + 2*k3.domega + k4.domega)


bob = sphere(radius=0.1, color=color.red)
rod = cylinder(radius=0.01, color=color.white)


graph_phase_space = graph(title="Phase Space", width=800, height=400, xtitle="theta", ytitle="omega")
gc = gdots(graph=graph_phase_space, color=color.blue)

theta_slider = slider(min=-math.pi, max=math.pi, value=math.pi/4, length=200, bind=None, right=15)
theta_text = wtext(text=f" Theta = {theta_slider.value:.2f} rad  ")

omega_slider = slider(min=-5, max=5, value=0, length=200, bind=None, right=15)
omega_text = wtext(text=f" Omega = {omega_slider.value:.2f} rad/s  ")

pause = False

def update_labels():
    theta_text.text = f" Theta = {theta_slider.value:.2f} rad  "
    omega_text.text = f" Omega = {omega_slider.value:.2f} rad/s  "

def change_simulation(evt):
    global pendulum_state, t
    pendulum_state.theta = theta_slider.value
    pendulum_state.omega = omega_slider.value
    t = 0
    update_labels()

def reset_simulation(evt):
    global pendulum_state, t
    pendulum_state.theta = math.pi/4
    pendulum_state.omega = 0
    t = 0
    gc.delete()
    update_labels()

def pause_simulation(evt):
    global pause
    pause = not pause

button(text="Change Condition", bind=change_simulation)
button(text="Reset Simulation", bind=reset_simulation)
button(text="Pause Simulation", bind=pause_simulation)

pendulum_state = PendulumState(theta_slider.value, omega_slider.value)
t = 0

while True:
    if pause:
        continue

    rate(100)
    RK4(pendulum_state, dt)
    t += dt

    x = L * math.sin(pendulum_state.theta)
    y = -L * math.cos(pendulum_state.theta)

    if (pendulum_state.theta > 2*math.pi):
        pendulum_state.theta = -pendulum_state.theta
    elif (pendulum_state.theta < -2*math.pi):
        pendulum_state.theta = -pendulum_state.theta

    bob.pos = vector(x, y, 0)
    rod.pos = pivot
    rod.axis = bob.pos - pivot
    update_labels()
    gc.plot(pos=(pendulum_state.theta, pendulum_state.omega))