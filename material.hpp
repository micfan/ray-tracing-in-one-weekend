#pragma once

#include <memory>
#include "vec3.hpp"
#include "ray.hpp"

struct hit_record; // forward declare

class material
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

using material_ptr = std::shared_ptr<material>;