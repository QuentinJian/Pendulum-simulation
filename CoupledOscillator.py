from vpython import *

m = 1.0      
K_R = 25
K_M = 10
K_L = 25     
dt = 0.001   
t = 0
t_max = 50


class State:
    def __init__(self, x1=0, v1=0, x2=0, v2=0):
        self.x1 = x1
        self.v1 = v1
        self.x2 = x2
        self.v2 = v2

class Derivative:
    def __init__(self, dx1=0, dv1=0, dx2=0, dv2=0):
        self.dx1 = dx1
        self.dv1 = dv1
        self.dx2 = dx2
        self.dv2 = dv2


scene = canvas(title="Coupled Oscillators", width=800, height=600, background=vec(0.5,0.5,0.5))

floor = box(size=vector(10, 0.01, 2), texture=textures.metal)
left_wall = box(size=vector(0.01, 1, 2), texture=textures.metal)
left_wall.pos = vec(-floor.size.x/2 - left_wall.size.x/2, left_wall.size.y/2 + floor.size.y/2, 0)

right_wall = box(size=vector(0.01, 1, 2), texture=textures.metal)
right_wall.pos = vec(floor.size.x/2 + right_wall.size.x/2, right_wall.size.y/2 + floor.size.y/2, 0)

block1 = box(size=vector(0.5, 0.5, 0.5), color=color.blue)
block2 = box(size=vector(0.5, 0.5, 0.5), color=color.red)

block1.pos = vec(-1, block1.size.y/2 + floor.size.y/2, 0)
block2.pos = vec(1,  block2.size.y/2 + floor.size.y/2, 0)

spring_left = helix(radius=0.1, coils=15, thickness=0.01)
spring_left.pos = vec(left_wall.pos.x + left_wall.size.x/2, block1.pos.y, 0)
spring_left.L0 = spring_left.length

spring_middle = helix(radius=0.1, coils=15, thickness=0.01)
spring_middle.pos = block1.pos
spring_middle.L0 = spring_middle.length

spring_right = helix(radius=0.1, coils=15, thickness=0.01)
spring_right.pos = block2.pos
spring_right.L0 = spring_right.length

gdx = graph(title="Position vs Time", width=1200, height=400, x=0, y=400, ytitle="x(m)", fast=False)
x_block1 = gcurve(graph=gdx, color=color.blue)
x_block2 = gcurve(graph=gdx, color=color.red)

pos1 = block1.pos.x
pos2 = block2.pos.x
v1 = 0.0
v2 = 0.0


def evaluate(initial: State, t: float, dt: float, d: Derivative) -> Derivative:
    state = State()
    state.x1 = initial.x1 + d.dx1 * dt
    state.v1 = initial.v1 + d.dv1 * dt
    state.x2 = initial.x2 + d.dx2 * dt
    state.v2 = initial.v2 + d.dv2 * dt
    
    
    output = Derivative()
    output.dx1 = state.v1
    output.dx2 = state.v2
    
    x_left = state.x1 - (left_wall.pos.x + left_wall.size.x/2)
    x_mid = (state.x2 - state.x1)
    x_right = (right_wall.pos.x - right_wall.size.x/2) - state.x2
    
    F_left = -K_R*(x_left - spring_left.L0)
    F_mid_1 = K_M*(x_mid - spring_middle.L0)
    F_mid_2 = -K_M*(x_mid - spring_middle.L0)
    F_right = K_L*(x_right - spring_right.L0)
    
    F1 = F_left + F_mid_1
    F2 = F_right + F_mid_2
    
    output.dv1 = F1/m
    output.dv2 = F2/m
    
    return output

def RK4(state: State, t: float, dt: float) -> State:
    k1 = evaluate(state, t, 0, Derivative())
    k2 = evaluate(state, t + dt/2, dt/2, k1)
    k3 = evaluate(state, t + dt/2, dt/2, k2)
    k4 = evaluate(state, t + dt, dt, k3)
    
    state.x1 += (dt/6.0) * (k1.dx1 + 2*k2.dx1 + 2*k3.dx1 + k4.dx1)
    state.v1 += (dt/6.0) * (k1.dv1 + 2*k2.dv1 + 2*k3.dv1 + k4.dv1)
    state.x2 += (dt/6.0) * (k1.dx2 + 2*k2.dx2 + 2*k3.dx2 + k4.dx2)
    state.v2 += (dt/6.0) * (k1.dv2 + 2*k2.dv2 + 2*k3.dv2 + k4.dv2)
    
    return state



state = State(block1.pos.x, 0, block2.pos.x, 0)

while t < t_max:
    rate(1000)
    
    state = RK4(state, t, dt)
    
    block1.pos.x = state.x1
    block2.pos.x = state.x2
    
    spring_left.axis = block1.pos - spring_left.pos
    spring_middle.pos = block1.pos
    spring_middle.axis = block2.pos - block1.pos
    spring_right.pos = block2.pos
    spring_right.axis = vec(right_wall.pos.x - right_wall.size.x/2, block2.pos.y, 0) - block2.pos
    
    x_block1.plot(pos=(t, state.x1))
    x_block2.plot(pos=(t, state.x2))
    
    t += dt