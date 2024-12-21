from vpython import *

g = 9.8
dt  = 0.0001
t = 0
K = 1000.0

class State:
    def __init__(self, pos=vector(0, 0, 0), v=vector(0, 0, 0), a=vector(0, 0, 0)) -> None:
        self.pos = pos
        self.v = v
        self.a = a

class Derivative:
    def __init__(self, dr=vector(0, 0, 0), dv=vector(0, 0, 0))->None:
        self.dr = dr
        self.dv = dv

    def __add__(self, other):
        dr = self.dr + other.dr
        dv = self.dv + other.dv
        return Derivative(dr, dv)
    
    def __mul__(self, other):
        dr = self.dr * other
        dv = self.dv * other
        return Derivative(dr, dv)
    
    def __truediv__(self, other):
        dr = self.dr / other
        dv = self.dv / other
        return Derivative(dr, dv)
    

def collide(v1, v2, m1, m2):
    v1f = v1*(m1-m2)/(m1+m2) + v2*2*m2/(m1+m2)
    v2f = v1*2*m1/(m2+m1) + v2*(m2-m1)/(m2+m1)
    return v1f, v2f

def acceleration(ball, F:vector):
    return F/ball.m


def evaluate(initial:State, t, dt, derivative:Derivative)->Derivative:
    state = State()
    state.pos = initial.pos + derivative.dr*dt
    state.v = initial.v + derivative.dv*dt
    
    output = Derivative()
    output.dr = state.v
    
    line_vec = state.pos - pinpoint.pos
    line_len = mag(line_vec)
    line_direct = line_vec/line_len
    F1 = -K * (line_len-line1.L) * line_direct + vector(0, -g*ball1.m, 0)
    output.dv = F1/ball1.m
    return output

def RK4(state:State, t, h):
    k1 = evaluate(state, t, 0, Derivative())
    k2 = evaluate(state, t, h/2, k1)
    k3 = evaluate(state, t, h/2, k2)
    k4 = evaluate(state, t, h, k3)
    
    total_d = (k1 + k2*2 + k3*2 + k4)/6
    state.pos = state.pos + total_d.dr*h 
    state.v = state.v + total_d.dv*h
    t+=h
    return state, t


dt = 0.001  

def calculate_energy(state):
    kinetic_energy = 0.5 * ball1.m * mag(state.v)**2
    potential_energy = ball1.m * g * state.pos.y
    return  kinetic_energy + potential_energy

g2 = graph(title='Total Energy')
energy_curve = gcurve(color=color.blue)


scene = canvas(centor=vector(0, -1.0, 0), background=vector(0.5, 0.5, 0))
pinpoint = sphere(radius=0.1, color=color.green, make_trail=False)
ball1 = sphere(radius=0.1, color=color.red, make_trail=False)

g1 = graph(title='phase space', xtitle='p', ytitle='x')
gc = gcurve(color=color.black)

ball1.pos = vector(2, 0, 0)
ball1.m = 0.5
ball1.v = vector(0, 0, 0)

line1 = cylinder(radius=0.01)
line1.pos = pinpoint.pos
line1.axis = ball1.pos - pinpoint.pos
line1.L = line1.length

ball_state = State(ball1.pos, ball1.v, vec(0, 0, 0))


while True:
    rate(1000)
    ball_state, t = RK4(ball_state, t, dt)
    ball1.pos = ball_state.pos
    line1.axis = ball1.pos - line1.pos
    
    # Plot phase space and energy
    gc.plot(ball_state.v.x/ball1.m, ball_state.pos.x)
    energy_curve.plot(t, calculate_energy(ball_state))
