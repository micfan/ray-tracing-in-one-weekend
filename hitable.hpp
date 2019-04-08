#pragma once

#include <memory>
#include "ray.hpp"

class material; // forward declare
using material_ptr = std::shared_ptr<material>;


struct hit_record
{
    double t;
    vec3 p;
    vec3 normal;
    material_ptr mat_ptr;
};

class hitable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
