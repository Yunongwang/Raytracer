#ifndef MATERIAL_H
#define MATERIAL_H
#include "hittable.h"

class material
{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3 &a) : albedo(a) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();

        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3 &a, float f) : albedo(a)
    {
        if (f < 1)
        {
            fuzz = f;
        }
        else
        {
            fuzz = 1;
        }
    }

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
    float fuzz;
};

class blinn : public material
{
public:
    blinn(const vec3 &_base, const vec3 &_lightpositon, const vec3 &_lightcolor)
    {
        base = _base;
        lightpositon = unit_vector(_lightpositon);
        lightcolor = _lightcolor;
        shiness = 10.0;
        kSpecular = 0.8;
        kDiffuse = 0.8;
    };

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
        //vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();

        scattered = ray(rec.p, target - rec.p);
        // scattered = ray(rec.p, halfVector);
        float NLAngle = std::max(dot(rec.normal, lightpositon), 0.0);
        vec3 diffuse = lightcolor * NLAngle * base;
        vec3 specular = vec3(0.0, 0.0, 0.0);
        //std::cout << NLAngle << std::endl;

        if (NLAngle > 0.0)
        {
            vec3 V = unit_vector(-r_in.direction()); // Vector to viewer
            vec3 H = unit_vector((V + lightpositon) * 0.5);
            // Compute the specular term
            float specAmount = pow(std::max(dot(rec.normal, H), 0.0), shiness);
            specular = specAmount * lightcolor;
        }
        attenuation = kSpecular * specular + kDiffuse * diffuse + base * 0.3;

        return true;
    }

    vec3 base, lightpositon, lightcolor;
    float shiness, kSpecular, kAmbient, kDiffuse;
};

class dielectric : public material
{
public:
    dielectric(float ri) : ref_idx(ri) {}

    virtual bool scatter(
        const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0)
        {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        bool can_refract = refract(r_in.direction(), outward_normal, ni_over_nt, refracted);
        if (can_refract)
        {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else
        {
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0;
        }
        if (!can_refract || (rand() / (RAND_MAX + 1.0)) < reflect_prob)
        {
            scattered = ray(rec.p, reflected);
        }
        else
        {
            scattered = ray(rec.p, refracted);
        }
        return true;
    }
    float ref_idx;
};

#endif //MATERIAL_H_