#pragma once
#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct _vec { //define a new structure called vec (vector)
    double x;
    double y;
} vec;


vec add_vec(vec *a, vec *b);
vec minus_vec(vec *a, vec *b);
vec times_vec(vec *a, double times);
vec polar_coord(double length, double radian); //transfer from polar coord to position vector ralative to origin
double norm(vec *a);
double dot(vec *a, vec *b);

#endif