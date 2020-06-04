#ifndef _SPHERE_H
#define _SPHERE_H

#include <cmath>
#include "Vec.h"
#include "Material.h"

class Sphere{
public:
    Vec center;                           
    double radius;    
    Material material;     
    
    //Normal Spheres
    Sphere(Vec c, double r, Material mat);
    //Declaration for Light Class 
    Sphere(Vec c, double r, Vec emit_color);

    bool intersect(const Vec &rayorig, const Vec &raydir, double &r1, double &r2) const;
};

Sphere::Sphere(Vec c, double r, Material mat){
    material = mat;
    center = c;
    radius = r;
}

Sphere::Sphere(Vec c, double r, Vec emit_color){
    center = c;
    radius = r;
    material.emitted_color = emit_color;
}

bool Sphere::intersect(const Vec &rayorig, const Vec &raydir, double &r1, double &r2) const{
        double a = raydir.dot(raydir);
        double b = (center - rayorig).dot(raydir);
        double c = (center - rayorig).dot(center - rayorig);
        double d = a*c-b*b;
        if(d-radius*radius > 0 || b < 0)
            return false;
        r2 = (b+sqrt(radius*radius - d));
        r1 = (b-sqrt(radius*radius - d));
        if(r1 < 0){
            r1 = r2;
        }
        return true; 
}


#endif