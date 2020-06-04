#ifndef _Plane_H
#define _Plane_H
#include <iostream>
#include "Vec.h"
#include "Sphere.h"

class Plane : public Sphere{
    public: 
    Plane(Vec c, Material material):
        Sphere(c, 10000, material){}
};


#endif