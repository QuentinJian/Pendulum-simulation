#pragma once
#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct _vec { //define a new structure called vec (vector)
    double x;
    double y;
} vec;


vec add_vec(struct vec *a, struct vec *b);
vec minus_vec(struct vec *a, struct vec *b);
double norm(vec *a);
double dot(struct vec *a, struct vec *b);

#endif