#pragma once

#include "hitable.hpp"
#include "material.hpp"

double schlick(double cosine, double ref_idx)
{
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1- r0)*pow((1 - cosine), 5);
}

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
        double reflect_prob;
        double cosine;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
            cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = - dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, reflected)) {
            reflect_prob = schlick(cosine, ref_idx);
        } else {
            reflect_prob = 1.0;
        }

        if ((rand() % 100 / 100.0) < reflect_prob) {
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, reflected);

        }
        return true;
    }

    double ref_idx;
};

