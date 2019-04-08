#pragma once

#include "hitable.hpp"
#include "material.hpp"

class sphere : public hitable
{
public:
    sphere() = delete;
    sphere(vec3 cen, double r, material_ptr mat) : center(std::move(cen)), radius(r), mat_ptr(mat) {}
    bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    vec3 center;
    double radius;
    material_ptr mat_ptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b*b - a*c;

    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}
