#ifndef _LIGHT_H
#define _LIGHT_H

#include "Vec.h"
#include "Sphere.h"

class Light : public Sphere{
    public:
    Light(Vec c, double r, Vec emit_color): 
        Sphere(c, r, emit_color){}
};

#endif
