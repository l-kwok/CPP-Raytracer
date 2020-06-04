#ifndef _VEC_H
#define _VEC_H

class Vec{
public:
    double x, y, z;
    Vec();
    Vec(double val);
    Vec(double x_val, double y_val, double z_val);
    Vec& normalize();
    double dot(const Vec &v) const;
    Vec operator * (const double &f) const { return Vec(x * f, y * f, z * f); }
    Vec operator * (const Vec &v) const { return Vec(x * v.x, y * v.y, z * v.z); }
    Vec operator - (const Vec &v) const { return Vec(x - v.x, y - v.y, z - v.z); }
    Vec operator + (const Vec &v) const { return Vec(x + v.x, y + v.y, z + v.z); }
    Vec operator - () const { return Vec(-x, -y, -z); } 
};
Vec::Vec(){
    x = 0;
    y = 0;
    z = 0; 
}

Vec::Vec(double val){
    x = val;
    y = val;
    z = val;
}

Vec::Vec(double x_val, double y_val, double z_val){
    x = x_val;
    y = y_val;
    z = z_val;
}

Vec& Vec::normalize(){
    double magnitude = sqrt((x*x) + (y*y) + (z*z));
    x = x/magnitude;
    y = y/magnitude;
    z = z/magnitude;
    return *this;
}

double Vec::dot(const Vec &v) const{
    return (x * v.x + y * v.y + z * v.z);
}
#endif