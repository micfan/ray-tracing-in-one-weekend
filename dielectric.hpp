#pragma once

#include "hitable.hpp"
#include "material.hpp"

class dielectric : public material
{
public:
    dielectric() = delete;

    explicit dielectric(double ri) : ref_idx(ri) {}

    bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const override
    {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        double ni_over_nt;
        attenuation = vec3{1.0, 1.0, 1.0};
        vec3 refracted;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, reflected)) {
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, reflected);
            return false;
        }

        return true;
    }

    double ref_idx;
};

