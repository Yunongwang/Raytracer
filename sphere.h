#ifndef SPHERE_H
#define SPHERE_H
#include <memory>
#include "hittable.h"

class sphere : public hit_table
{
public:
    sphere() {}
    sphere(vec3 cen, float r, material *m)
    {
        center = cen;
        radius = r;
        mat = m;
    }
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override;
    vec3 center;
    float radius;
    material *mat;
};

bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp > t_min && t_max > temp)
        {
            rec.t = temp;
            rec.p = r.point_at_paramter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp > t_min && t_max > temp)
        {
            rec.t = temp;
            rec.p = r.point_at_paramter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }
    }

    return false;
    //     vec3 oc = r.origin() - center;
    // float a = r.direction().squared_length();
    // float b = dot(oc, r.direction());
    // float c = oc.squared_length() - radius*radius;
    // float discriminant = b*b -a*c;
    // if(discriminant < 0) return false;
    // float sqrtd = sqrt(discriminant);
    // float root = (-b-sqrtd)/a;
    // if (root < t_min || t_max < root) {
    //     root = (-b +sqrtd)/a;
    //     if (root<t_min||t_max<root)
    //         return false;
    // }
    // rec.t = root;
    // rec.p = r.point_at_paramter(rec.t);
    // vec3 outward_normal = (rec.p - center) / radius;
    // rec.set_face_normal(r, outward_normal);
    // return true;
}

#endif //SPHERE_H_