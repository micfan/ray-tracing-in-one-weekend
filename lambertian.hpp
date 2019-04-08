#pragma once

#include "vec3.hpp"
#include "ray.hpp"
#include "hitable.hpp"
#include "material.hpp"

extern vec3 random_in_unit_sphere();

class lambertian : public material
{
public:
    explicit lambertian(const vec3& a) : albedo(a) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray{rec.p, target - rec.p};
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

    vec3 albedo;
};
