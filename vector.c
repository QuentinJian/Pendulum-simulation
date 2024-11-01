#include "vector.h"
#include <math.h>

vec add_vec(vec a, vec b) {
    vec result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

vec minus_vec(vec *a, vec *b) {
    vec result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    return result;
} 

vec polar_coord(double length, double radian) {
    vec position;
    position.x = length*cos(radian);
    position.y = length*sin(radian);
    return position;
}

vec scale_vec(vec a, double times) {
    vec result;
    result.x = a.x * times;
    result.y = a.y * times;
    return result;
}

vec norm(vec a) {
    vec unit;
    unit.x = a.x / mag(a);
    unit.y = a.y / mag(a);
    return unit;
}

double mag(vec a) {
    return sqrt(pow(a.x, 2) + pow(a.y, 2));
}

double dot (vec *a, vec *b) {
    return (a->x)*(b->x) + (a->y)*(b->y);
}