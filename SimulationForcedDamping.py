from vpython import *


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
    
m = 2
v0 = 1

K = 25
i = 0
t = 0
dt = 0.001

scene = canvas(title="circle", width=800, height=600, background=vec(0.5,0.5,0.5))
floor = box(size=vector(5, 0.01, 2), texture=textures.metal)

wall=box(size=vector(0.01, 1, 2), texture=textures.metal)
wall.pos = vec(-floor.size.x/2-wall.size.x/2, wall.size.y/2+floor.size.y/2, 0)

square = box(size=vector(0.5, 0.5, 0.5), texture=textures.wood, v=vec(v0, 0, 0), a=vec(0, 0, 0))
square.pos = vec(0, square.size.y/2+floor.size.y/2, 0)

spring = helix(radius=0.1, coils=15, thickness=0.01)
spring.pos = vec(wall.pos.x+0.01/2, square.pos.y, 0)
spring.axis = square.pos - spring.pos
spring.L0 = spring.length

gdx = graph(title="x-t plot", width=400, height=200, x=0, y=400, ytitle="x(m)", xmax=100)
xt = gcurve(graph=gdx, color=color.green)
gdv = graph(title="v-t plot", width=400, height=200, x=0, y=400, ytitle="v(m)", xmax=20)
vt = gcurve(graph=gdv, color=color.red)
gda = graph(title="a-t plot", width=400, height=200, x=0, y=400, ytitle="a(m)", xmax=20)
at = gcurve(graph=gda, color=color.blue)

v_ar = arrow(shaftwidth=0.01, color=color.green)
v_la = label(text='v', box=0, opacity=0)

a_ar = arrow(shaftwidth=0.01, color=color.red)
a_la = label(text='a', box=0, opacity=0)


def periodic_force(F0, omega, t, phase):
    return F0 * cos(omega*t + phase)

def evaluate(initial:State, t, dt, derivative:Derivative)->Derivative:
    state = State()
    state.pos = initial.pos + derivative.dr*dt
    state.v = initial.v + derivative.dv*dt
    
    output = Derivative()
    output.dr = state.v
    
    spring_force = -K * (state.pos.x - spring.L0) 
    damping_force = -b * state.v.x  
    driving_force = periodic_force(4, 1, t+dt, 0) 
    total_acceleration = (spring_force + damping_force + driving_force) / m
    
    output.dv = vector(total_acceleration, 0, 0)
    state.a = output.dv

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

def update_arrow(thing, n):
    a_ar.axis = thing.a
    a_ar.pos = thing.pos
    a_la.pos = thing.pos + thing.a/n
    a_la.text = 'A' + ' ' + str(thing.a)
    v_ar.axis = thing.v
    v_ar.pos = thing.pos
    v_la.pos = thing.pos + thing.v/n
    v_la.text= 'V' + ' ' + str(thing.v)

def plot_graph(thing):
    xt.plot(pos=(t, thing.pos.x))
    vt.plot(pos=(t, thing.v.x))
    at.plot(pos=(t, thing.a.x))

square_pos = State(square.pos, square.v, square.a)
b = 0.5  # damping coefficient

while(t<50):
    rate(1000)
    
    square_pos, t = RK4(square_pos, t, dt)

    left_limit = wall.pos.x + wall.size.x/2 + square.size.x/2
    if square_pos.pos.x < left_limit:
        square_pos.pos.x = left_limit
        square_pos.v.x = -square_pos.v.x * 0.8
    
    square.pos = square_pos.pos
    square.v = square_pos.v
    square.a = square_pos.a
    
    spring.axis = square.pos - spring.pos
    
    plot_graph(square)
    update_arrow(square, 5)
    t += dt
    