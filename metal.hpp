#pragma once

#include "material.hpp"
#include "vec3.hpp"


vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n)*n;
}

class metal : public material
{
public:
    explicit metal(const vec3& a, double f = 0.1) : albedo(a), fuzz( f < 1 ? f : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray{rec.p, reflected + fuzz * random_in_unit_sphere()};
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

    vec3 albedo;
    double fuzz;
};
