#include "vector.h"
#include <math.h>

vec add_vec(vec *a, vec *b) {
    vec result;
    result.x = a->x + b->x;
    result.y = a->y + b->y;
    return result;
}

vec minus_vec(vec *a, vec *b) {
    vec result;
    result.x = a->x - b->x;
    result.y = a->y - b->y;
    return result;
} 

double norm(vec *a) {
    return sqrt(pow(a->x, 2) + pow(a->y, 2));
}

double dot (vec *a, vec *b) {
    return (a->x)*(b->x) + (a->y)*(b->y);
}