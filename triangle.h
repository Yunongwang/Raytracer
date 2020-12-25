#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <memory>
#include "hittable.h"
#include <iostream>



class triangle: public hit_table {
    public:
        triangle() {}
        triangle(vec3 _a, vec3 _b, vec3 _c,  material* m) {a = _a; b =_b; c = _c; mat = m;
        n = unit_vector(cross(_b-_a, _c-_a));}
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
        vec3 a, b, c, n;
        material* mat;
};

bool triangle::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    float check = dot(r.direction(),n);
    if (check < 0.001 && check > -0.001){
        return false;
    }

    vec3 w = a - r.origin();
    float fa = dot(w,n);
    float fb = dot(r.direction(),n);
    float k = fa/fb;
    vec3 i = (r.direction()*k +r.origin());
    //std::cout << k << std::endl;;
    float t = (cross(a-b, a-c)).length();
    float aa = (cross(i-b, i-c)).length();
    float ba = (cross(i-a, i-c)).length();
    float ca = (cross(i-a, i-b)).length();
    float checktri = (aa + ba + ca)/t -1.0;

    if (checktri > 0.001 || checktri < -0.001){
        return false;
    }
    //std::cout << checktri << std::endl;;
    if(k > t_min && t_max > k){
        rec.t = k;
        rec.p = r.point_at_paramter(rec.t);
        rec.normal = n;

        rec.mat_ptr = mat;
        return true;
    }

    return false;

}

#endif //TRIANGLE_H_