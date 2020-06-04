#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "Vec.h"

class Material : public Vec{
    public: 
    Vec color, emitted_color; //[R,G,B] for each vec
    double opacity, shininess;
    Material();
    Material(Vec c, Vec emit_c, double o, double s);
    Material(Vec c, double o, double s);
};

Material::Material(){
    color = Vec();
    emitted_color = Vec();
    opacity = 0;
    shininess = 0;
}

Material::Material(Vec c, Vec emit_c, double o, double s){
    opacity = o;
    shininess = s;
    color = c;
    emitted_color = emit_c;
}

Material::Material(Vec c, double o, double s){
    opacity = o;
    shininess = s; 
    color = c; 
}

#endif